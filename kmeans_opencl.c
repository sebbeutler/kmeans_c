#include "kmeans_opencl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <CL/cl.h>

#include "data_structures/clist.h"
#include "ui.h"

#define MAX_SOURCE_SIZE (0x100000)

#define MAX_POINTS 100
#define MAX_SPECIES 4

typedef struct Point
{
    double x;
    double y;
    int specie;
} Point;

typedef struct Specie
{
	int id;
	int color[3];
    Point centroid;
	Point points[MAX_POINTS];
	int point_count;
} Specie;


Point points[MAX_POINTS];
cl_mem points_mem;
Specie species[MAX_SPECIES];
cl_mem species_mem;


cl_int ret = 0;
cl_program program;
cl_command_queue command_queue;
cl_kernel kernel;
size_t global_item_size[3];
size_t local_item_size[3];

cl_program load_program(cl_context context, char* filename, cl_int* errcode_ret);

bool opencl_draw()
{
    SDL_SetRenderDrawColor( ui_renderer, 240, 185, 36, 255 );

    for (int i=0; i < MAX_SPECIES; i++)
    {
        for (int j=0; j < species[i].point_count; j++)
        {
            SDL_SetRenderDrawColor( ui_renderer, species[i].color[0], species[i].color[1], species[i].color[2], 255 );
            DrawCircle( ui_renderer, species[i].points[j].x, species[i].points[j].y, 10);
        }
        SDL_SetRenderDrawColor( ui_renderer, species[i].color[0], species[i].color[1], species[i].color[2], 255 );
        DrawCircle( ui_renderer, species[i].centroid.x, species[i].centroid.y, 30); 
    }
    return true; // continue?
}

void opencl_event(SDL_Event* e)
{
    switch ( e->type ) {
        case SDL_KEYUP:
            if ( e->key.keysym.sym == SDLK_SPACE)
            {
                kernel = clCreateKernel(program, "kmeans_step", &ret);
                unsigned long initstate = (unsigned long)time(NULL);
                global_item_size[0] = 1;
                local_item_size[0] = 1;
                ret |= clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_item_size, local_item_size, 0, NULL, NULL);
                
                // Read buffers
                ret |= clEnqueueReadBuffer(command_queue, points_mem, CL_TRUE, 0, sizeof(points), points, 0, NULL, NULL);
                ret |= clEnqueueReadBuffer(command_queue, species_mem, CL_TRUE, 0, sizeof(species), species, 0, NULL, NULL);

                clReleaseKernel(kernel);
            }
    }
}

char BIN_PATH[255];
void kmeans_opencl(int argc, char *argv[])
{
    strcpy(BIN_PATH, argv[0]);
    for (size_t i = strlen(BIN_PATH) - 1; i >= 0; i--)
    {
        if (BIN_PATH[i] == '/' || BIN_PATH[i] == '\\')
        {
            BIN_PATH[i + 1] = '\0';
            break;
        }
    }

#pragma region OPENCL_INIT

    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    ret |= clGetPlatformIDs(1, &platform_id, NULL);
    ret |= clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, NULL);
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
    
    size_t info;
    clGetDeviceInfo(
    device_id,
    CL_DEVICE_MAX_WORK_GROUP_SIZE,
    sizeof(info),
    &info,
    NULL);

    // for (int i=0; i < 3; i++)
    //     printf("Info: %llu\n", info[i]);

    // printf("Info: %llu\n", info);

#pragma endregion OPENCL_INIT

#pragma region KERNEL_EXEC 

    program = load_program(context, "kmeans.cl", &ret);
    ret |= clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    
    size_t len = 0;
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
    char *buffer = calloc(len, sizeof(char));
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, len, buffer, NULL);
    printf("%s\n", buffer);
    
    memset(points, 0, sizeof(points));
    points_mem =  clCreateBuffer(
        context,
        CL_MEM_COPY_HOST_PTR,
        sizeof(points),
        points,
        &ret);
    
    memset(species, 0, sizeof(species));
    species_mem =  clCreateBuffer(
        context,
        CL_MEM_COPY_HOST_PTR,
        sizeof(species),
        species,
        &ret);

    
    // Run kernel > main
    kernel = clCreateKernel(program, "kmeans_init", &ret);
    ret |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &points_mem);
    ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &species_mem);
    unsigned long initstate = (unsigned long)time(NULL);
    #pragma warning (disable : 4311)
    unsigned long initseq = (unsigned long)&printf;
    ret |= clSetKernelArg(kernel, 2, sizeof(unsigned long), &initstate);
    ret |= clSetKernelArg(kernel, 3, sizeof(unsigned long), &initseq);
    global_item_size[0] = 1;
    local_item_size[0] = 1;
    ret |= clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_item_size, local_item_size, 0, NULL, NULL);
    
    // Read buffers
    ret |= clEnqueueReadBuffer(command_queue, points_mem, CL_TRUE, 0, sizeof(points), points, 0, NULL, NULL);
    ret |= clEnqueueReadBuffer(command_queue, species_mem, CL_TRUE, 0, sizeof(species), species, 0, NULL, NULL);

    clReleaseKernel(kernel);


#pragma endregion KERNEL_EXEC

#pragma region KERNEL_POST_EXEC

    ui_init();
    ui_run(&opencl_draw, &opencl_event);

#pragma endregion KERNEL_POST_EXEC


#pragma region OPENCL_CLEAN

    printf("\n");
    if (ret == 0)
        printf("opencl success.\n");
    else
        printf("opencl error: [%d]\n", ret);

    // clean

    ret |= clReleaseMemObject(points_mem);
    ret |= clReleaseMemObject(species_mem);

    clFlush(command_queue);
    clFinish(command_queue);
    clReleaseCommandQueue(command_queue);
    clReleaseProgram(program);    
    clReleaseContext(context);

#pragma endregion OPENCL_CLEAN
}

cl_program load_program(cl_context context, char* filename, cl_int* errcode_ret)
{
    FILE *fp;
    char *source_str;
    size_t source_size;


    char kernel_path[255];
    strcpy(kernel_path, BIN_PATH);
    strcat(kernel_path, filename);
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    if (!source_str)
    {
        *errcode_ret |= -4;
        return 0;
    }
    fp = fopen(kernel_path, "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    return clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, errcode_ret);
}

