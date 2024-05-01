#include "helpers.h"
#include <math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //go through every row
    for(int i = 0; i < height; i++)
    {
        //go through every column
        for(int j = 0; j < width; j++)
        {
            float red = image[i][j].rgbtRed;
            float green =image[i][j].rgbtGreen;
            float blue = image[i][j].rgbtBlue;
            // Take average of red, green, and blue
            int average = round((red+green+blue)/3);
            // Update pixel values
            image[i][j].rgbtRed = image[i][j].rgbtGreen =image[i][j].rgbtBlue = average;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
     // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float originalRed = image[i][j].rgbtRed;
            float originalGreen =image[i][j].rgbtGreen;
            float originalBlue = image[i][j].rgbtBlue;
            // Compute sepia values
            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            int sepiaGreen =round( .349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int sepiaBlue =round( .272 * originalRed + .534 * originalGreen + .131 * originalBlue);
            // Update pixel with sepia values
            if(sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if(sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if(sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Swap pixels
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - (j +1)];
            image[i][width - (j + 1)] = temp;

        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int totalRed, totalBlue, totalGreen;
            totalRed = totalBlue = totalGreen = 0;
            float counter = 0.00;

            //get current postion of pixels
            for(int x = -1; x < 2; x++)
            {
                for(int y = -1; y < 2; y++)
                {
                    int currentX = i + x;
                    int currentY = j + y;

                    //check if the pixels are valid
                    if(currentX < 0 || currentX > (height - 1) || currentY < 0 || currentY > (width -1))
                    {
                        continue;
                    }

                    totalRed += image[currentX][currentY].rgbtRed;
                    totalGreen +=  image[currentX][currentY].rgbtGreen;
                    totalBlue +=  image[currentX][currentY].rgbtBlue;
                     counter++;
                }
                //calculate average of neighbouring pixels
                copy[i][j].rgbtRed = round(totalRed / counter);
                copy[i][j].rgbtGreen = round(totalGreen / counter);
                copy[i][j].rgbtBlue = round(totalBlue / counter);
            }


        }

    }
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = copy[i][j].rgbtRed;
            image[i][j].rgbtGreen = copy[i][j].rgbtGreen;
            image[i][j].rgbtBlue = copy[i][j].rgbtBlue;
        }
    }
    return;
}
