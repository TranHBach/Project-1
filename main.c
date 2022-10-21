#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

// RGB is : 20 255 10
void mask_image(unsigned char *image, int width, int height, int channel)
{
    // width is: number of cols
    // Height is: number of rows
    // So to process image from top to bottom, left to right, the height must be in the outer for loop.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int location = i * width * channel + j * channel;
            // if (image[location] <= 150 && image[location + 1] >= 200 && image[location + 2] <= 150)
            if (image[location + 1] * 1.5 >= (image[location] + image[location + 2]))
            {
                for (int k = 0; k < channel; k++)
                {
                    image[location + k] = 0;
                }
            }
        }
    }
}

void combine_image(unsigned char *image, unsigned char *mask, int width, int height, int channel) {
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int location = i * width * channel + j * channel;
            // if (image[location] <= 150 && image[location + 1] >= 200 && image[location + 2] <= 150)
            if (mask[location] != 0 && mask[location + 1] != 0 && mask[location + 2] != 0)
            {
                for (int k = 0; k < channel; k++)
                {
                    image[location + k] = mask[location + k];
                }
            }
        }
    }
}

int main()
{
    int width, height, channel;
    int temp1, temp2, temp3;
    char path_img[] = "./images/98239648_p0.png";
    char save_path[] = "./images/98239648_p0_new.png";

    char green_screen_img[] = "./images/good_green_screen.jpg";

    unsigned char *image = stbi_load(path_img, &width, &height, &channel, 0);

    // Both have same size so we can ignore the image info
    unsigned char *mask = stbi_load(green_screen_img, &temp1, &temp2, &temp3, 0);

    printf("%d %d %d\n", width, height, channel);

    mask_image(mask, width, height, channel);
    printf("Finish masking\n");

    combine_image(image, mask, width, height, channel);
    printf("Finish combining\n");

    stbi_write_png(save_path, width, height, channel, image, width * channel);
    printf("Finished");

}