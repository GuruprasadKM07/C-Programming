#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *stego_image_fname;  // store argv[2], file name
    FILE *fptr_stego_image;   // File pointer for source image file
    uint image_capacity;    // sizeof bmp file
    //uint bits_per_pixel;
    //char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    /*char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    //char secret_data[MAX_SECRET_BUF_SIZE];*/
    int size_secret_file;

    /* Stego Image Info */
    char out_image_fname[20];
    FILE *fptr_out_image;
    int out_file_size;

} DecodeInfo;


/* Encoding function prototype */

/* Check operation type */
//OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_bmp_file(DecodeInfo *decInfo);

// Decode magic string
Status decode_magic_string(FILE *fptr_stego_image);

// Function to decode extn size
Status decode_secret_file_extn_size(FILE *fptr_stego_image, DecodeInfo *decInfo);

// Function to decode secret file extn
Status decode_secret_file_extn(int extn_size, DecodeInfo *decInfo);

//Function to open output file
Status open_output_file(DecodeInfo *decInfo);

// Function decode secret file size
Status decode_secret_file_size(FILE *fptr_stego_image, DecodeInfo *decInfo);

// Function to decode secret data
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode function, which does the real decoding */
Status decode_data_from_image(FILE *fptr_stego_image, int size, char *str);

/* Decode a byte into LSB of image data array */
char decode_byte_from_lsb(FILE *fptr_stego_image);
#endif
