/**
 * @file tkl_ota.c
 * @brief default weak implements of tuya ota, it only used when OS=linux
 * 
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 * 
 */
#include <string.h>
#include "tkl_ota.h"
#include "tuya_error_code.h"
#include "tkl_memory.h"
#include "tkl_flash.h"
#include "tkl_system.h"
#include "tkl_watchdog.h"
#include "tuya_cloud_types.h"

#define FLASH_SECTOR_SIZE 4096
#define BLOCK_SZ FLASH_SECTOR_SIZE

#define FIRMWARE_AREA           1
#define PATCH_AREA              2
#define BACKUP_AREA             3
#define MANAGER_AREA            4
#define LAST_WRITE_FIRMWARE     5
#define INVALID_ARG             255


#define TO_PHYSICS   1
#define TO_VIRTUAL   0
#define BK_ADDR_CHANGE(_TYPE_,_ADDR_)  ((((_TYPE_) == TO_PHYSICS) ? ((_ADDR_) / 32) * 34 : ((_ADDR_) / 34) * 32))
#define v_to_p(a) ((a / 32) * 34 + a % 32)
#define VIRTUAL_TO_AB_PHYSICS(addr) (v_to_p(addr) - v_to_p(addr) % BLOCK_SZ)

#define len_v_to_p(a) ((a / 32) * 34 + ((a % 32) ? 34 : 0))



typedef enum {
    UGS_RECV_HEADER = 0,
    UGS_RECV_IMG_DATA,
    UGS_RECV_IMG_DATA_B,
    UGS_FINISH
}UG_STAT_E;

typedef enum {
    TYPE_DIFF = 0,
    TYPE_SEG_A,
    TYPE_SEG_B
}OTA_TYPE_E;


typedef struct {
    uint32_t start_addr;
    uint32_t cur_addr;
    uint32_t recv_data_cnt;
    uint32_t bin_len;
    uint32_t bin_type;
    uint32_t offset;
    uint8_t firmware_is_crc;  //0- 固件区不需要加crc校验， 1- 固件区需要加crc校验（bk平台）
    UG_STAT_E stat;
    OTA_TYPE_E ota_type;//0-diff; 1-seg_A; 2-seg_B
}UG_PROC_S;

extern void bk_printf(const char *fmt, ...);
/***********************************************************
*************************variable define********************
***********************************************************/
#define SEC_B_UNIT (1024)
#define FLASH_BASE_ADDR (0x02000000)

static uint32_t flash_area = INVALID_ARG;
static UG_PROC_S *ug_proc = NULL;
static uint8_t tkl_fist_flag = 0;

/**
* @brief erase flash
*
* @param[in] addr: flash address
* @param[in] size: size of flash block
*
* @note This API is used for erasing flash.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tkl_ota_flash_erase(uint32_t addr, uint32_t size, void* arg)
{
    uint32_t param = *((uint32_t*)arg);
    uint32_t real_size = size;
    uint32_t address = addr;

    if(param == FIRMWARE_AREA) {
        uint32_t phy_size =  (size % 32) ? (size / 32 * 34 + 34) : (size / 32 * 34);
        uint32_t off_size = BK_ADDR_CHANGE(TO_PHYSICS, addr) % FLASH_SECTOR_SIZE;
        real_size = ((off_size+phy_size)%FLASH_SECTOR_SIZE) ? ((((off_size+phy_size)/FLASH_SECTOR_SIZE)+1)*FLASH_SECTOR_SIZE) : (off_size+phy_size);
        address = BK_ADDR_CHANGE(TO_PHYSICS, addr) - off_size;
    } else {
        real_size = (size%FLASH_SECTOR_SIZE) ? ((size/FLASH_SECTOR_SIZE) + 1)*FLASH_SECTOR_SIZE : size;
    }
    //bk_printf("tkl_ota_flash_erase:%x %d, %d\r\n",address, param, real_size);
    tkl_watchdog_refresh();
    //tkl_flash_set_protect(FALSE);
    tkl_flash_erase(address, real_size);//这里的擦除的size 是否需要改为：off_size+len_v_to_p(size)
    //tkl_flash_set_protect(TRUE);
    tkl_watchdog_refresh();

    return OPRT_OK;

}

/**
* @brief read flash
*
* @param[in] addr: flash address
* @param[out] dst: pointer of buffer
* @param[in] size: size of buffer
*
* @note This API is used for reading flash.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tkl_ota_flash_read(uint32_t addr, uint8_t *dst, uint32_t size)
{
    return tkl_flash_read(addr, dst, size);
}

//extern void ty_calc_crc(uint32_t *buf, uint32_t packet_num);
//extern void ty_get_calc_buf(uint32_t *rx, uint32_t *tx, uint32_t num);
//extern void ty_encrypt(uint32_t *rx, uint16_t *tx, uint32_t num,uint32_t  addr0);

static uint8_t bbuf[1024 / 32 * 34 + 34];
static uint8_t tbuf[32];
OPERATE_RET tkl_ota_flash_write(uint32_t addr, uint8_t *buf, uint32_t len, void* arg)
{
    uint32_t param = *((uint32_t*)arg);
    static uint8_t last_data_cnt = 0;
    //bk_printf("tkl_ota_flash_write:%x, %d, %d\r\n",addr,len, param);

    if(param == FIRMWARE_AREA || param == LAST_WRITE_FIRMWARE) {//write firmware
        uint32_t off = 0;
        uint32_t encrypt_num = 0;
        uint32_t lcnt = len;
        uint32_t address = addr;
        //bk_printf("last cnt:%d, len:%d\r\n",last_data_cnt, len);
        if(last_data_cnt) {
            if((lcnt + last_data_cnt) >= 32) {
                memcpy(tbuf+last_data_cnt, buf, 32-last_data_cnt);
                //ty_get_calc_buf(tbuf, bbuf, 1);
                //ty_encrypt((uint32_t *)tbuf, bbuf, 1, BK_ADDR_CHANGE(TO_PHYSICS, address-last_data_cnt));
                //ty_calc_crc((uint32_t *)(bbuf), 1);
                lcnt -= 32-last_data_cnt;
                off += 32-last_data_cnt;
                last_data_cnt = 0;
                address -= last_data_cnt;
                tkl_watchdog_refresh();
                //tkl_flash_set_protect(FALSE);
                if(tkl_flash_write(BK_ADDR_CHANGE(TO_PHYSICS, address), bbuf, 34)) {
                    //tkl_flash_set_protect(TRUE);
                    bk_printf("Write sector failed\r\n");
                    return OPRT_OS_ADAPTER_OTA_PROCESS_FAILED;
                }
                //tkl_flash_set_protect(TRUE);
                tkl_watchdog_refresh();
                address += 32;
            } 
        }
        while(lcnt > 32) {
            encrypt_num = (lcnt > SEC_B_UNIT) ? (SEC_B_UNIT / 32) : lcnt / 32;

            //ty_encrypt((uint32_t *)(buf+off), bbuf, encrypt_num, BK_ADDR_CHANGE(TO_PHYSICS, address));
            //ty_calc_crc((uint32_t *)bbuf, encrypt_num);
            tkl_watchdog_refresh();

            //tkl_flash_set_protect(FALSE);
            if(tkl_flash_write(BK_ADDR_CHANGE(TO_PHYSICS, address), bbuf, encrypt_num*34)) {
                //tkl_flash_set_protect(TRUE);
                bk_printf("Write sector failed\r\n");
                return OPRT_OS_ADAPTER_OTA_PROCESS_FAILED;
            }
            //tkl_flash_set_protect(TRUE);
            tkl_watchdog_refresh();
            lcnt -= encrypt_num * 32;
            off += encrypt_num * 32;
            address += encrypt_num * 32;
        }  

        if(lcnt && param != LAST_WRITE_FIRMWARE) {  
            memcpy(tbuf + last_data_cnt, (buf+off), lcnt);
            last_data_cnt += lcnt;
        } else if(lcnt && param == LAST_WRITE_FIRMWARE) {
            bk_printf("last_data_cnt:%d\r\n", last_data_cnt);
            uint8_t restbuf[32];
            memset(restbuf, 0xff, 32);
            memcpy(restbuf, tbuf, last_data_cnt);
            memcpy(restbuf + last_data_cnt, buf+off, lcnt);
  
            //ty_encrypt((uint32_t *)restbuf, bbuf, 1, BK_ADDR_CHANGE(TO_PHYSICS, address - last_data_cnt));
            //ty_calc_crc((uint32_t *)bbuf, 1);
            tkl_watchdog_refresh();
            //tkl_flash_set_protect(FALSE);
            if(tkl_flash_write(BK_ADDR_CHANGE(TO_PHYSICS, address - last_data_cnt), bbuf, 34)) {
                //tkl_flash_set_protect(TRUE);
                bk_printf("Write sector failed\r\n");
                return OPRT_OS_ADAPTER_OTA_PROCESS_FAILED;
            }
            //tkl_flash_set_protect(TRUE);
            tkl_watchdog_refresh();
        }
        
    }else {
        tkl_watchdog_refresh();
        //tkl_flash_set_protect(FALSE);
        tkl_flash_write(addr, buf, len);
        //tkl_flash_set_protect(TRUE);
        tkl_watchdog_refresh();
    }

    return OPRT_OK;
}

/**
* @brief ota start notify
*
* @param[in] image_size:  image size
* @param[in] type:        ota type
* @param[in] path:        ota path
*
* @note This API is used for ota start notify
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tkl_ota_start_notify(uint32_t image_size, TUYA_OTA_TYPE_E type, TUYA_OTA_PATH_E path)
{
    if(image_size == 0) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    if(ug_proc == NULL) {
        ug_proc = tkl_system_malloc(sizeof(UG_PROC_S));
        if(NULL == ug_proc) {
            return OPRT_MALLOC_FAILED;
        }
    }
    memset(ug_proc, 0,sizeof(UG_PROC_S));
    ug_proc->bin_len = image_size;

    ug_proc->ota_type = TYPE_DIFF;
    if(path == TUYA_OTA_PATH_SEC_A) {
        ug_proc->ota_type = TYPE_SEG_A;
    } else if(path == TUYA_OTA_PATH_SEC_B) {
        ug_proc->ota_type = TYPE_SEG_B; 
        ug_proc->stat = UGS_RECV_IMG_DATA_B;
    }

    return 0;

    //bk_printf("_ota_start_notify:%d, %d, %d, %d\r\n",type, ug_proc->bin_len, ug_proc->ota_type,ug_proc->stat);
}


/**
* @brief ota data process
*
* @param[in] pack:       point to ota pack
* @param[in] remain_len: ota pack remain len
*
* @note This API is used for ota data process
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tkl_ota_data_process(TUYA_OTA_DATA_T *pack, uint32_t* remain_len)
{
    if(ug_proc == NULL) {
        bk_printf("ota don't start or start err,process error!\r\n");
        return OPRT_COM_ERROR;
    }

    if(ug_proc->ota_type == TYPE_SEG_A)
        return -1;

    //bk_printf("ug_proc->stat:%d\r\n",ug_proc->stat);
    if(ug_proc->stat == UGS_RECV_IMG_DATA_B) {
        if(!tkl_fist_flag) {
            tkl_fist_flag = 1;

            uint8_t *temp_buf = NULL;
            uint32_t off_size = pack->start_addr % FLASH_SECTOR_SIZE;
            uint32_t address = pack->start_addr - off_size;
            //bk_printf("off_size:%x,%x,%x\r\n",off_size, address, pack->start_addr);
            if(off_size != 0) {
                temp_buf = (uint8_t *) tkl_system_malloc(off_size);
                if(NULL == temp_buf) {
                    return OPRT_MALLOC_FAILED;
                }
            }

            if(off_size != 0) {
                tkl_ota_flash_read(address, temp_buf, off_size);
            }

            flash_area = FIRMWARE_AREA;
            tkl_ota_flash_erase(pack->start_addr, ug_proc->bin_len, (void *)&flash_area);
            
            if(off_size != 0) {
                flash_area = INVALID_ARG;
                tkl_ota_flash_write(address, temp_buf, off_size, (void *)&flash_area);
            }
            if(NULL != temp_buf) {
                tkl_system_free(temp_buf);
                temp_buf = NULL;
            }
        }

        flash_area = FIRMWARE_AREA;
        if(ug_proc->recv_data_cnt + pack->len >= ug_proc->bin_len) {
            pack->len = ug_proc->bin_len - ug_proc->recv_data_cnt;
            bk_printf("recv finish222222\r\n");
            flash_area = LAST_WRITE_FIRMWARE;
        }

        //bk_printf("start_addr:%x\r\n",pack->start_addr);
        tkl_ota_flash_write(pack->start_addr, pack->data, pack->len, (void *)&flash_area);
        ug_proc->recv_data_cnt += pack->len;
        *remain_len = 0;
    } else {
        if(!tkl_fist_flag) {
            tkl_fist_flag = 1;
            flash_area = PATCH_AREA;
            tkl_ota_flash_erase(pack->start_addr, ug_proc->bin_len, (void *)&flash_area);
        }
        flash_area = PATCH_AREA;
        tkl_ota_flash_write(pack->start_addr, pack->data, pack->len, (void *)&flash_area);
    }
    
    return OPRT_OK;
}

/**
 * @brief firmware ota packet transform success inform, can do verify in this funcion
 * 
 * @param[in]        reset       need reset or not
 * 
 * @return OPRT_OK on success, others on failed
 */
OPERATE_RET tkl_ota_end_notify(BOOL_T reset)
{
    tkl_fist_flag = 0;
    return OPRT_OK;
}


/**
* @brief get ota ability
*
* @param[out] image_size:  max image size
* @param[out] type:        bit0, 1 - support full package upgrade
                                 0 - dont support full package upgrade
*                          bit1, 1 - support compress package upgrade
                                 0 - dont support compress package upgrade
*                          bit2, 1 - support difference package upgrade
                                 0 - dont support difference package upgrade
* @note This API is used for get chip ota ability
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tkl_ota_get_ability(uint32_t *image_size, TUYA_OTA_TYPE_E *type)
{
    *image_size = 1500 * 1024;
#if defined(ENABLE_MATTER) && (ENABLE_MATTER == 1)
        *type = TUYA_OTA_DIFF;  // only support diff ota for matter
#else
        *type = TUYA_OTA_FULL;
#endif

    return OPRT_OK;
}



