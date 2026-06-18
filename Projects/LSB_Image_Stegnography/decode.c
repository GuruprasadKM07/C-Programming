#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "common.h"
#include "types.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(!strcmp(".bmp",strstr(argv[2],".bmp")))
    {
	decInfo -> stego_image_fname = argv[2];
	printf("%s\n",decInfo -> stego_image_fname);
    }
    else
    {
	printf("Pass valid args\n");
	return e_failure;
    }

    if(argv[3] != NULL)
    {
	strcpy(decInfo -> out_image_fname,argv[3]);
	printf("%s\n",decInfo -> out_image_fname);
    }
    else
    {
	strcpy(decInfo -> out_image_fname,"output");
	printf("%s\n",decInfo -> out_image_fname);
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if(open_bmp_file(decInfo) == e_success)
    {
	printf("INFO: Done\n");
	sleep(1);
	if(decode_magic_string(decInfo -> fptr_stego_image) == e_success)
	{
	    printf("INFO: Done\n");
	    sleep(1);
	    if(decode_secret_file_extn_size(decInfo -> fptr_stego_image, decInfo) == e_success)
	    {
		printf("INFO: Done\n");
		sleep(1);
		if(decode_secret_file_extn(decInfo -> out_file_size, decInfo) == e_success)
		{
		    printf("INFO: Done\n");
		    sleep(1);
		    if(open_output_file(decInfo) == e_success)
		    {
			printf("INFO: Done\n");
			sleep(1);
			if(decode_secret_file_size(decInfo -> fptr_stego_image, decInfo) == e_success)
			{
			    printf("INFO: Done\n");
			    sleep(1);
			    if(decode_secret_file_data(decInfo) == e_success)
			    {
				printf("INFO: Done\n");
				sleep(1);
			    }
			    else
			    {
				printf("Failure in decoding secret data\n");
				return e_failure;
			    }
			}
			else
			{
			    printf("Failure in decoding secret file size\n");
			    return e_failure;
			}
		    }
		    else
		    {
			printf("Failure in opening output file\n");
			return e_failure;
		    }
		}
		else
		{
		    printf("Failure in extn decding\n");
		    return e_failure;
		}
	    }
	    else
	    {
		printf("Failure in extn size decoding\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("Failure in decoing\n");
	    return e_failure;
	}
    }
    else
    {
	printf("Failure in file opening\n");
	return e_failure;
    }
    return e_success;
}

/* Function to open bmp file*/
Status open_bmp_file(DecodeInfo *decInfo)
{
    printf("INFO: Opening required files\n");
    sleep(1);
    // Open stego image file
    decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname,"r");

    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

	return e_failure;
    }

    printf("INFO: Opened stego.bmp file\n");
    sleep(1);
    // No Error return e_success
    return e_success;
}

// Function to decode magic string
Status decode_magic_string(FILE *fptr_stego_image)
{
    printf("INFO: Decoding Magic String Signature\n");
    sleep(1);

    rewind(fptr_stego_image);
    fseek(fptr_stego_image, 54, SEEK_SET);
    //printf("%ld\n",ftell(fptr_stego_image));
    char buffer[2];
    if(decode_data_from_image(fptr_stego_image, strlen(MAGIC_STRING), buffer) == e_success)
    {
	printf("%s\n",buffer);
	for(int i=0; i<2; i++)
	{
	    if(MAGIC_STRING[i] != buffer[i])
		return e_failure;
	}
    }
    return e_success;
}

Status decode_secret_file_extn_size(FILE *fptr_stego_image, DecodeInfo *decInfo)
{
    printf("INFO: Decoding output file extension size\n");
    sleep(1);

    unsigned int val, res = 0;
    for(int i=0; i<32; i++)
    {
	val = decode_byte_from_lsb(fptr_stego_image);
	res = res | (val << i);
    }
    
    decInfo -> out_file_size = res;
    //printf("Extn size is %d\n",decInfo -> out_file_size);
    return e_success;
}

Status decode_secret_file_extn(int extn_size, DecodeInfo *decInfo)
{
    printf("INFO: Decoding output file extension\n");
    sleep(1);

    char buffer[extn_size];
    if(decode_data_from_image(decInfo -> fptr_stego_image, extn_size, buffer) == e_success)
    {
	//printf("%s\n",buffer);
	strcat(decInfo -> out_image_fname, buffer);
	//printf("%s\n",decInfo -> out_image_fname);
	//decInfo -> out_image_fname[strlen(decInfo -> out_image_fname)] = '\0';
	//printf("%s\n",decInfo -> out_image_fname);
	//printf("output file is %s\n", decInfo -> out_image_fname);
	return e_success;
    }
    return e_failure;
}

Status open_output_file(DecodeInfo *decInfo)
{
    printf("INFO: Opening output file\n");
    sleep(1);

    // Open stego image file
    decInfo -> fptr_out_image = fopen(decInfo -> out_image_fname,"w");

    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

	return e_failure;
    }

    // No Error return e_success
    return e_success;
}

Status decode_secret_file_size(FILE *fptr_stego_image, DecodeInfo *decInfo)
{
    printf("INFO: Decoding output file size\n");
    sleep(1);

    unsigned int val, res = 0;
    for(int i=0; i<32; i++)
    {
	val = decode_byte_from_lsb(fptr_stego_image);
	res = res | (val << i);
    }
    
    decInfo -> size_secret_file = res;
    //printf("File size is %d\n",decInfo -> size_secret_file);
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    printf("INFO: Decoding secret data\n");
    sleep(1);

    char buffer[decInfo -> size_secret_file];
    if(decode_data_from_image(decInfo -> fptr_stego_image, decInfo -> size_secret_file, buffer) == e_success)
    {
	fwrite(buffer, decInfo -> size_secret_file, 1, decInfo -> fptr_out_image);
	fclose(decInfo -> fptr_out_image);
	//printf("%s\n",buffer);
	return e_success;
    }
    return e_failure;
}

Status decode_data_from_image(FILE *fptr_stego_image, int size, char *str)
{
    int i;
    char val, res;
    for(i=0; i<size; i++)
    {
	val = 0;
	res = 0;
	for(int j=0; j<8; j++)
	{
	    val = decode_byte_from_lsb(fptr_stego_image);
	    res = res | (val << j);
	}
	str[i] = res;
    }
    str[i] = '\0';
    return e_success;
}

char decode_byte_from_lsb(FILE *fptr_stego_image)
{
    char val;
    fread(&val, 1, 1, fptr_stego_image);
    val = val & 1;
    return val;
}
