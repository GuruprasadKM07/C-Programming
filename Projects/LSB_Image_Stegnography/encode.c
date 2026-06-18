#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "common.h"
#include "types.h"

/* Function Definitions */
// Function to check operation type
// Function of read and evaluate inputs
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    char *sub_ptr1=strstr(argv[2],".bmp");
    char *sub_ptr2=strstr(argv[3],".txt");

    // STEP1
    if(sub_ptr1!=NULL)
    {
	if(!strcmp(sub_ptr1,".bmp"))
	    encInfo->src_image_fname=argv[2];
	else
	    return e_failure;
    }
    else
    {
	printf("Please pass valid source file\n");
	return e_failure;
    }

    // STEP2
    if(sub_ptr2!=NULL)
    {
	if(!strcmp(sub_ptr2,".txt"))
	{
	    encInfo->secret_fname=argv[3];
	    strcpy(encInfo -> extn_secret_file, sub_ptr2);
	}
	else
	    return e_failure;
    }
    else
    {
	printf("Please pass valid secret file\n");
	return e_failure;
    }

    // STEP3
    if(argv[4])
    {
	char *sub_ptr3=strstr(argv[4],".bmp");
	if(sub_ptr3!=NULL)
	{
	    if(!strcmp(sub_ptr3,".bmp"))
		encInfo->stego_image_fname=argv[4];
	    else
	    {
		printf("Error please pass .bmp output file \n");
		return e_failure;
	    }
	}
	else
	{
	    printf("Please pass valid output file\n");
	    return e_failure;
	}
    }
    else
	encInfo->stego_image_fname="output.bmp";

    return e_success;

}
/* Encode begins with do_encoding fun
   Inside first we need to check the total sizeof
   source file in bytes */
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
	printf("INFO: Done\n");
	sleep(1);

	printf("INFO: ######## Encoding Procedure Started ########\n");
	sleep(1);
	if(check_capacity(encInfo) == e_success )
	{
	    printf("INFO: Done. Found OK\n");
	    sleep(1);
	    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
	    {
		printf("INFO: Done\n");
		sleep(1);
		//printf("file pointer = %ld\n", ftell(encInfo -> fptr_src_image));
		if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
		{
		    printf("INFO: Done\n");
		    sleep(1);
		    //printf("file pointer = %ld\n", ftell(encInfo -> fptr_src_image));
		    if(encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo) == e_success)
		    {
			printf("INFO: Done\n");
			sleep(1);
			//printf("file pointer = %ld\n", ftell(encInfo -> fptr_src_image));
			if(encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_success)
			{
			    printf("INFO: Done\n");
			    sleep(1);
			    //printf("file pointer = %ld\n", ftell(encInfo -> fptr_src_image));
			    if(encode_secret_file_size(encInfo -> size_secret_file, encInfo) == e_success)
			    {
				printf("INFO: Done\n");
				sleep(1);
				//printf("file pointer = %ld\n", ftell(encInfo -> fptr_src_image));
				if(encode_secret_file_data(encInfo) == e_success)
				{
				    printf("INFO: Done\n");
				    sleep(1);
				    //printf("file pointer = %ld\n", ftell(encInfo -> fptr_src_image));
				    if(copy_remaining_img_data(encInfo) == e_success)
				    {
					printf("INFO: Done\n");
					sleep(1);
					//printf("file pointer = %ld\n", ftell(encInfo -> fptr_src_image));
				    }
				    else
				    {
					printf("Failure in remaining data encoding\n");
					return e_failure;
				    }
				}
				else
				{
				    printf("Failure in secret data encding\n");
				    return e_failure;
				}
			    }
			    else
			    {
				printf("Failure in file size encoding\n");
				return e_failure;
			    }
			}
			else
			{
			    printf("Failur in extn encoding\n");
			    return e_failure;
			}
		    }
		    else
		    {
			printf("Failure in extn_size\n");
			return e_failure;
		    }
		}
		else
		{
		    printf("Failure in MS Encoding\n");
		    return e_failure;
		}
	    }
	    else
	    {
		printf("Failure in copy header\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("Capacity is there \n");
	    return e_failure;
	}
    }
    else
    {
	printf("Files are not open\n");
	return e_failure;
    }
    return e_success;
}

/* Checking capacity of file in bytes*/
Status check_capacity(EncodeInfo *encInfo)
{
    printf("INFO: Checking for beautiful.bmp file capacity\n");
    sleep(1);

    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
    //printf("Size of extn %zu\n", strlen(encInfo -> extn_secret_file));
    //printf("Image capacity %d\n",encInfo -> image_capacity);
    if(encInfo -> image_capacity > (56+16+32+(strlen(encInfo -> extn_secret_file)*8)+32+(encInfo -> size_secret_file *8)))
	return e_success;
    else
	return e_failure;

}
/* Getting size of secret file*/
uint get_file_size(FILE *fptr)
{
    rewind(fptr);
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

/* Copying the header of bmp to secret stego file*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    printf("INFO: Copying image header\n");
    sleep(1);

    char arr_header[54];
    rewind(fptr_src_image);
    fread(arr_header, sizeof(arr_header), 1, fptr_src_image);	

    rewind(fptr_stego_image);
    fwrite(arr_header, sizeof(arr_header), 1, fptr_stego_image);
    return e_success;
}

/* Function to encode magic string*/
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    printf("INFO: Encoding Magic String Signature\n");
    sleep(1);
    if(encode_data_to_image(MAGIC_STRING, strlen(MAGIC_STRING), encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success )
	return e_success;
}

/* Real encoding function */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    for(int i=0; i<size; i++)
    {
	char image_buffer[8];
	fread(image_buffer, 8, 1, fptr_src_image);
	encode_byte_to_lsb(data[i], image_buffer);
	fwrite(image_buffer, 8, 1, fptr_stego_image);
    }
    return e_success;
}

/* Function to encode extn size*/
Status encode_secret_file_extn_size(int size_file_extn, EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret.txt file extn size\n");
    sleep(1);

    char size_arr[32];
    fread(size_arr, 32, 1, encInfo -> fptr_src_image);
    for(int i=0; i<32; i++)
    {
	encode_byte_to_lsb_size(&size_arr[i],(size_file_extn>>i) & 1);
	fwrite(&size_arr[i], 1, 1, encInfo -> fptr_stego_image);
    }
    return e_success;
}
/* Function to encode secret_file extn */

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret.txt File Extension\n");
    sleep(1);
    /*int size = strlen(file_extn);
      for(int i=0; i<size; i++)
      {
      char extn_arr[8];
      fread(extn_arr, 8, 1, encInfo -> fptr_src_image);
      encode_byte_to_lsb(file_extn[i], extn_arr);
      fwrite(extn_arr, 8, 1, encInfo -> fptr_stego_image);
      }*/
    if(encode_data_to_image(file_extn, strlen(file_extn), encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
	return e_success;
    //return e_success;
}
/* Function to encode secret file size */

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret.txt file size\n");
    sleep(1);
    char file_size_buff[32];
    fread(file_size_buff, 32, 1, encInfo -> fptr_src_image);
    for(int i=0; i<32; i++)
    {
	encode_byte_to_lsb_size(&file_size_buff[i], (file_size>>i) & 1);
	fwrite(&file_size_buff[i], 1, 1, encInfo -> fptr_stego_image);
    }
    return e_success;
}
/* Function to encode secret file data */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("INFO: Encoding secret.txt File Data\n");
    sleep(1);
    //printf("size_secret_file %ld\n",encInfo -> size_secret_file);
    rewind(encInfo -> fptr_secret);
    /*for(int i=0; i<(encInfo -> size_secret_file); i++)
      {
      char data_arr[8];
      char buffer;
      fread(data_arr, 8, 1, encInfo -> fptr_src_image);
      fread(&buffer, 1, 1, encInfo -> fptr_secret);
      encode_byte_to_lsb(buffer, data_arr);
      fwrite(data_arr, 8, 1, encInfo -> fptr_stego_image);
      }
      return e_success;*/
    char buffer[encInfo -> size_secret_file];
    fread(buffer, encInfo -> size_secret_file, 1, encInfo -> fptr_secret);
    if(encode_data_to_image(buffer, encInfo -> size_secret_file, encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
	return e_success;
}

/* Function to encode remaining data */

Status copy_remaining_img_data(EncodeInfo *encInfo)
{
    printf("INFO: Copying left Over Data\n");
    sleep(1);
    /*int remaining_size = ((encInfo -> image_capacity) - (56+16+32+(strlen(encInfo -> extn_secret_file)*8)+32+(encInfo->size_secret_file*8)))+2;
      char buffer[remaining_size];
      fread(&buffer, remaining_size, 1, encInfo -> fptr_src_image);
      fwrite(&buffer, remaining_size, 1, encInfo -> fptr_stego_image);*/
    char ch = 0;
    while(fread(&ch, 1, 1, encInfo -> fptr_src_image) > 0)
    {
	fwrite(&ch, 1, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}
/* Function to encode lsb byte*/
void encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0; i<8; i++)
    {
	image_buffer[i] = (image_buffer[i] & -2) | ((data >> i) & 1);
    }
    /*for(int i=0; i<8; i++)
    {
	printf("%d\n",image_buffer[i]);
    }*/
}

void encode_byte_to_lsb_size(char *arr, int size_bit)
{
    *arr = (*arr & -2) | size_bit;
}
/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}
/*uint get_image_size_for_bmp(FILE *fptr_image)
  {
  rewind(fptr_image);
  fseek(fptr_image, 0, SEEK_END);
  return ftell(fptr_image);
  }*/

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    printf("INFO: Opening required files\n");
    sleep(1);

    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

	return e_failure;
    }

    printf("INFO: Opened beautiful.bmp\n");
    sleep(1);


    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

	return e_failure;
    }

    printf("INFO: Opened secret.txt\n");
    sleep(1);

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

	return e_failure;
    }

    printf("INFO: Opened stego.bmp\n");
    sleep(1);
    // No failure return e_success
    return e_success;
}
