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
                for (int k = 0; k < channel-1; k++)
                {
                    image[location + k] = 0;
                }
            }
        }
    }
}

void combine_image(unsigned char *image, unsigned char *mask, int width, int height, int channel, int m_width, int m_height, int m_channel) {
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int i_location = i * width * channel + j * channel;
            int m_location = i * m_width * m_channel + j * m_channel;
            // if (image[location] <= 150 && image[location + 1] >= 200 && image[location + 2] <= 150)
            if (mask[m_location] != 0 && mask[m_location + 1] != 0 && mask[m_location + 2] != 0)
            {
                for (int k = 0; k < channel; k++)
                {
                    image[i_location + k] = mask[m_location + k];
                }
            }
        }
    }
}

int main()
{
    int width, height, channel;
    int m_width, m_height, m_channel;
    char path_img[] = "./images/weather_forecast.jpg";
    char save_path[] = "./images/weather_forecast_new.jpg";

    char green_screen_img[] = "./images/woman_in_greenscreen.png";

    unsigned char *image = stbi_load(path_img, &width, &height, &channel, 0);

    // Both have same size so we can ignore the image info
    unsigned char *mask = stbi_load(green_screen_img, &m_width, &m_height, &m_channel, 0);

    // Size of mask
    printf("%d %d %d\n", m_width, m_height, m_channel);

    // Size of image
    printf("%d %d %d\n", width, height, channel);

    // Removing green screen
    mask_image(mask, m_width, m_height, m_channel);
    printf("Finish masking\n");

    // Combining the mask and image
    combine_image(image, mask, width, height, channel, m_width, m_height, m_channel);
    printf("Finish combining\n");

    // Write image
    stbi_write_png(save_path, width, height, channel, image, width * channel);
    printf("Finished");

}