#include "helpers.h"
#include <math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            float red = image[i][j].rgbtRed;
            float green = image[i][j].rgbtGreen;
            float blue = image[i][j].rgbtBlue;

            //find out the average
            int average = round((red + green + blue)/3);

            image[i][j].rgbtRed = image[i][j].rgbtGreen =image[i][j].rgbtBlue = average;

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for(int i = 0;i < height; i++)
    {
        for(int j = 0; j < width/2; j++)
        {
            temp= image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
   RGBTRIPLE copy[height][width];
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    int Gx[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
    int Gy[3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};
    
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int redX = 0;
            int greenX = 0;
            int blueX = 0;
            int redY = 0;
            int greenY = 0;
            int blueY = 0;

            //loop through each pixels
            for(int x = 0; x < 3; x++)
            {
                for(int y = 0; y < 3; y++)
                {
                    //check for valid pixels
                    if(i - 1 + x < 0 || i - 1 + x > height - 1 ||
                    j - 1 + y < 0 || j - 1 + y > width - 1)
                    {
                        continue;
                    }

                    //calculate Gx for each colour
                    redX += (image[i - 1 + x][j - 1 + x].rgbtRed * Gx[x][y]);
                    greenX += (image[i - 1 + x][j - 1 + x].rgbtGreen * Gx[x][y]);
                    blueX += (image[i - 1 + x][j - 1 + x].rgbtBlue * Gx[x][y]);
                    //calculate Gy for each color
                    redY += (image[i - 1 + x][j - 1 + x].rgbtRed * Gy[x][y]);
                    greenY += (image[i - 1 + x][j - 1 + x].rgbtGreen * Gy[x][y]);
                    blueY += (image[i - 1 + x][j - 1 + x].rgbtBlue * Gy[x][y]);
                }
            }

            //calculate sqaure root of gx and gy
            int red = round(sqrt((redX * redX) + (redY * redY)));
            int green = round(sqrt((greenX * greenX) + (greenY * greenY)));
            int blue= round(sqrt((blueX * blueX) + (blueY * blueY)));

            if(red > 255)
            {
                red = 255;
            }
            if(green > 255)
            {
                green = 255;
            }
            if(blue > 255)
            {
                blue = 255;
            }
            //copy values into temp image
            temp[i][j].rgbtRed = red;
            temp[i][j].rgbtGreen = green;
            temp[i][j].rgbtBlue = blue;


        }
    }
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}
