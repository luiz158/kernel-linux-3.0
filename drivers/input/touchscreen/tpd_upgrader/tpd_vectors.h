#include "tpd_directives.h"

unsigned char target_status00_v = 0x00; // Status = 00 means Success, the SROM function did what it was supposed to 
unsigned char target_status01_v = 0x01; // Status = 01 means that function is not allowed because of block level protection
unsigned char target_status03_v = 0x03; // Status = 03 is fatal error, SROM halted
unsigned char target_status04_v = 0x04; // Status = 04 means that ___ for test with ___ (PROGRAM-AND-VERIFY) 
unsigned char target_status06_v = 0x06; // Status = 06 means that Calibrate1 failed, for test with id_setup_1 (ID-SETUP-1)

#ifdef CY8CTMA300_36LQXI
    unsigned char target_id_v[] = {0x05, 0x71}; //ID for CY8CTMA300_36LQXI
#endif
#ifdef CY8CTMA300_48LTXI
    unsigned char target_id_v[] = {0x05, 0x72}; //ID for CY8CTMA300_48LTXI
#endif
#ifdef CY8CTMA300_49FNXI
    unsigned char target_id_v[] = {0x05, 0x73}; //ID for CY8CTMA300_49FNXI
#endif
#ifdef CY8CTMA300B_36LQXI
    unsigned char target_id_v[] = {0x05, 0x74}; //ID for CY8CTMA300B_36LQXI
#endif
#ifdef CY8CTMA301D_36LQXI
    unsigned char target_id_v[] = {0x05, 0x77}; //ID for CY8CTMA301D_36LQXI
#endif
#ifdef CY8CTMA301D_48LTXI
    unsigned char target_id_v[] = {0x05, 0x78}; //ID for CY8CTMA301D_48LTXI
#endif
#ifdef CY8CTMA300D_36LQXI
    unsigned char target_id_v[] = {0x05, 0x79}; //ID for CY8CTMA300D_36LQXI
#endif
#ifdef CY8CTMA300D_48LTXI
    unsigned char target_id_v[] = {0x05, 0x80}; //ID for CY8CTMA300D_48LTXI
#endif
#ifdef CY8CTMA300D_49FNXI
    unsigned char target_id_v[] = {0x05, 0x81}; //ID for CY8CTMA300D_49FNXI
#endif
#ifdef CY8CTMA301E_36LQXI
    unsigned char target_id_v[] = {0x05, 0x85}; //ID for CY8CTMA301E_36LQXI
#endif
#ifdef CY8CTMA301E_48LTXI
    unsigned char target_id_v[] = {0x05, 0x86}; //ID for CY8CTMA301E_48LTXI
#endif
#ifdef CY8CTMA300E_36LQXI
    unsigned char target_id_v[] = {0x05, 0x82}; //ID for CY8CTMA300E_36LQXI
#endif
#ifdef CY8CTMA300E_48LTXI
    unsigned char target_id_v[] = {0x05, 0x83}; //ID for CY8CTMA300E_48LTXI
#endif
#ifdef CY8CTMA300E_49FNXI
    unsigned char target_id_v[] = {0x05, 0x84}; //ID for CY8CTMA300E_49FNXI
#endif
#ifdef CY8CTMA140_48LTXI
    unsigned char target_id_v[] = {0x05, 0xC3}; //ID for CY8CTMA140_48LTXI
#endif

const unsigned int num_bits_checksum = 418;
const unsigned char checksum_v[] =
{
    0xDE, 0xE2, 0x1F, 0x7F, 0x02, 0x7D, 0xC4, 0x09,
    0xF7, 0x00, 0x1F, 0x9F, 0x07, 0x5E, 0x7C, 0x81,
    0xF9, 0xF4, 0x01, 0xF7, 0xF0, 0x07, 0xDC, 0x40,
    0x1F, 0x70, 0x01, 0xFD, 0xEE, 0x01, 0xF7, 0xA0,
    0x1F, 0xDE, 0xA0, 0x1F, 0x7B, 0x00, 0x7D, 0xE0,
    0x0F, 0xF7, 0xC0, 0x07, 0xDF, 0x28, 0x1F, 0x7D,
    0x18, 0x7D, 0xFE, 0x25, 0xC0
};

const unsigned int num_bits_program_block = 440;
const unsigned char program_block[] =
{
    0xDE, 0xE2, 0x1F, 0x7F, 0x02, 0x7D, 0xC4, 0x09,
    0xF7, 0x00, 0x1F, 0x9F, 0x07, 0x5E, 0x7C, 0x81,
    0xF9, 0xF7, 0x01, 0xF7, 0xF0, 0x07, 0xDC, 0x40,
    0x1F, 0x70, 0x01, 0xFD, 0xEE, 0x01, 0xF6, 0xA0,
    0x0F, 0xDE, 0x80, 0x7F, 0x7A, 0x80, 0x7D, 0xEC,
    0x01, 0xF7, 0x80, 0x57, 0xDF, 0x00, 0x1F, 0x7C,
    0xA0, 0x7D, 0xF4, 0x61, 0xF7, 0xF8, 0x97
};

const unsigned int num_bits_id_setup_1 = 616;
const unsigned char id_setup_1[] =
{
    0xCA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0D, 0xEE, 0x21, 0xF7, 0xF0, 0x27, 0xDC, 0x40,
    0x9F, 0x70, 0x01, 0xFD, 0xEE, 0x01, 0xE7, 0xC1,
    0xD7, 0x9F, 0x20, 0x7E, 0x3F, 0x9D, 0x78, 0xF6,
    0x21, 0xF7, 0xB8, 0x87, 0xDF, 0xC0, 0x1F, 0x71,
    0x00, 0x7D, 0xC0, 0x07, 0xF7, 0xB8, 0x07, 0xDE,
    0x80, 0x7F, 0x7A, 0x80, 0x7D, 0xEC, 0x01, 0xF7,
    0x80, 0x4F, 0xDF, 0x00, 0x1F, 0x7C, 0xA0, 0x7D,
    0xF4, 0x61, 0xF7, 0xF8, 0x97
};

const unsigned int num_bits_id_setup_2 = 418;
const unsigned char id_setup_2[] =
{
    0xDE, 0xE2, 0x1F, 0x7F, 0x02, 0x7D, 0xC4, 0x09,
    0xF7, 0x00, 0x1F, 0x9F, 0x07, 0x5E, 0x7C, 0x81,
    0xF9, 0xF4, 0x01, 0xF7, 0xF0, 0x07, 0xDC, 0x40, 
    0x1F, 0x70, 0x01, 0xFD, 0xEE, 0x01, 0xF7, 0xA0,
    0x1F, 0xDE, 0xA0, 0x1F, 0x7B, 0x00, 0x7D, 0xE0,
    0x0D, 0xF7, 0xC0, 0x07, 0xDF, 0x28, 0x1F, 0x7D,
    0x18, 0x7D, 0xFE, 0x25, 0xC0			
};

const unsigned int num_bits_tsync_enable = 110;
const unsigned char tsync_enable[] =
{
    0xDE, 0xE2, 0x1F, 0x7F, 0x02, 0x7D, 0xC4, 0x09, 
    0xF7, 0x00, 0x1F, 0xDE, 0xE0, 0x1C 	
};
const unsigned int num_bits_tsync_disable = 110;
const unsigned char tsync_disable[] =
{			
    0xDE, 0xE2, 0x1F, 0x71, 0x00, 0x7D, 0xFC, 0x01, 
    0xF7, 0x00, 0x1F, 0xDE, 0xE0, 0x1C 
};

const unsigned int num_bits_set_block_num = 33;		
const unsigned char set_block_num[] =
{
    0xDE, 0xE0, 0x1E, 0x7D, 0x00, 0x70 
};
const unsigned int num_bits_set_block_num_end = 3;
const unsigned char set_block_num_end = 0xE0;

const unsigned int num_bits_read_write_setup = 66;
const unsigned char read_write_setup[] =
{
    0xDE, 0xF0, 0x1F, 0x78, 0x00, 0x7D, 0xA0, 0x03, 
    0xC0 
};

const unsigned int num_bits_my_verify_setup = 440;
const unsigned char verify_setup[] =
{
    0xDE, 0xE2, 0x1F, 0x7F, 0x02, 0x7D, 0xC4, 0x09,
    0xF7, 0x00, 0x1F, 0x9F, 0x07, 0x5E, 0x7C, 0x81,
    0xF9, 0xF7, 0x01, 0xF7, 0xF0, 0x07, 0xDC, 0x40,
    0x1F, 0x70, 0x01, 0xFD, 0xEE, 0x01, 0xF6, 0xA8,
    0x0F, 0xDE, 0x80, 0x7F, 0x7A, 0x80, 0x7D, 0xEC,
    0x01, 0xF7, 0x80, 0x0F, 0xDF, 0x00, 0x1F, 0x7C,
    0xA0, 0x7D, 0xF4, 0x61, 0xF7, 0xF8, 0x97
};

const unsigned int num_bits_erase = 396;		
const unsigned char erase[] =
{
    0xDE, 0xE2, 0x1F, 0x7F, 0x02, 0x7D, 0xC4, 0x09, 
    0xF7, 0x00, 0x1F, 0x9F, 0x07, 0x5E, 0x7C, 0x85, 
    0xFD, 0xFC, 0x01, 0xF7, 0x10, 0x07, 0xDC, 0x00, 
    0x7F, 0x7B, 0x80, 0x7D, 0xE0, 0x0B, 0xF7, 0xA0, 
    0x1F, 0xDE, 0xA0, 0x1F, 0x7B, 0x04, 0x7D, 0xF0, 
    0x01, 0xF7, 0xC9, 0x87, 0xDF, 0x48, 0x1F, 0x7F, 
    0x89, 0x70
};

const unsigned int num_bits_erase_block = 396;
const unsigned char erase_block[] =
{
    0xDE, 0xE2, 0x1F, 0x7F, 0x02, 0x7D, 0xC4, 0x09,
    0xF7, 0x00, 0x1F, 0x9F, 0x07, 0x5E, 0x7C, 0x85,
    0xFD, 0xFC, 0x01, 0xF7, 0x10, 0x07, 0xDC, 0x00,
    0x7F, 0x7B, 0x80, 0x7D, 0xE0, 0x07, 0xF7, 0xA0,
    0x1F, 0xDE, 0xA0, 0x1F, 0x7B, 0x04, 0x7D, 0xF0,
    0x01, 0xF7, 0xC9, 0x87, 0xDF, 0x48, 0x1F, 0x7F,
    0x89, 0x70
};

const unsigned int num_bits_secure = 440;
const unsigned char secure[] =
{
    0xDE, 0xE2, 0x1F, 0x7F, 0x02, 0x7D, 0xC4, 0x09,
    0xF7, 0x00, 0x1F, 0x9F, 0x07, 0x5E, 0x7C, 0x81,
    0xF9, 0xF7, 0x01, 0xF7, 0xF0, 0x07, 0xDC, 0x40,
    0x1F, 0x70, 0x01, 0xFD, 0xEE, 0x01, 0xF6, 0xA0,
    0x0F, 0xDE, 0x80, 0x7F, 0x7A, 0x80, 0x7D, 0xEC,
    0x01, 0xF7, 0x80, 0x27, 0xDF, 0x00, 0x1F, 0x7C,
    0xA0, 0x7D, 0xF4, 0x61, 0xF7, 0xF8, 0x97
};

const unsigned int num_bits_checksum_setup = 418;
const unsigned char checksum_setup[] =
{	
    0xDE, 0xE2, 0x1F, 0x7F, 0x02, 0x7D, 0xC4, 0x09,
    0xF7, 0x00, 0x1F, 0x9F, 0x07, 0x5E, 0x7C, 0x81,
    0xF9, 0xF4, 0x01, 0xF7, 0xF0, 0x07, 0xDC, 0x40,
    0x1F, 0x70, 0x01, 0xFD, 0xEE, 0x01, 0xF7, 0xA0,
    0x1F, 0xDE, 0xA0, 0x1F, 0x7B, 0x00, 0x7D, 0xE0,
    0x0F, 0xF7, 0xC0, 0x07, 0xDF, 0x28, 0x1F, 0x7D,
    0x18, 0x7D, 0xFE, 0x25, 0xC0		
};

const unsigned int num_bits_program_and_verify = 440;
const unsigned char program_and_verify[] =
{
    0xDE, 0xE2, 0x1F, 0x7F, 0x02, 0x7D, 0xC4, 0x09,
    0xF7, 0x00, 0x1F, 0x9F, 0x07, 0x5E, 0x7C, 0x81,
    0xF9, 0xF7, 0x01, 0xF7, 0xF0, 0x07, 0xDC, 0x40,
    0x1F, 0x70, 0x01, 0xFD, 0xEE, 0x01, 0xF6, 0xA0,
    0x0F, 0xDE, 0x80, 0x7F, 0x7A, 0x80, 0x7D, 0xEC,
    0x01, 0xF7, 0x80, 0x57, 0xDF, 0x00, 0x1F, 0x7C,
    0xA0, 0x7D, 0xF4, 0x61, 0xF7, 0xF8, 0x97
};

const unsigned int num_bits_verify_security = 440;
const unsigned char verify_security[] =
{		
    0xDE, 0xE2, 0x1F, 0x7F, 0x02, 0x7D, 0xC4, 0x09,
    0xF7, 0x00, 0x1F, 0x9F, 0x07, 0x5E, 0x7C, 0x81,
    0xF9, 0xF4, 0x01, 0xE7, 0xDC, 0x07, 0xDF, 0xC0,
    0x1F, 0x71, 0x00, 0x7D, 0xC0, 0x07, 0xF7, 0xB8, 
    0x07, 0xDE, 0x80, 0x7F, 0x7A, 0x80, 0x7D, 0xEC, 
    0x01, 0xF7, 0x80, 0x9F, 0xDF, 0x00, 0x1F, 0x7C, 
    0xA0, 0x7D, 0xF4, 0x61, 0xF7, 0xF8, 0x97 
};

const unsigned char read_id_v[] =
{
    0xBF, 0x00, 0xDF, 0x90, 0x00, 0xFE, 0x60, 0xFF, 
    0x00
};
        
const unsigned char write_byte_start = 0x90;    //this is set to SRAM 0x80
const unsigned char write_byte_end = 0xE0;

const unsigned char set_block_number[] = {0x9F, 0x40, 0xE0};
const unsigned char set_block_number_end = 0xE0;

const unsigned char num_bits_wait_and_poll_end = 40;
const unsigned char wait_and_poll_end[] = 
{  
    0x00, 0x00, 0x00, 0x00, 0x00 
};            
            
const unsigned char read_checksum_v[] = 
{  
    0xBF, 0x20, 0xDF, 0x80, 0x80
};

const unsigned char read_byte_v[] = 
{  
    0xB0, 0x80 
};    