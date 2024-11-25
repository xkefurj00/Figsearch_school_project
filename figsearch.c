#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILEPATH "obrazek.txt"

struct Image
{
   int number_of_lines;  // variable that stores number of lines
   int number_of_columns; // variable that stores number of columns
   char **colors; // field of bitmap colors
};


/*
This fuction allocating memory for image using malloc
First time for rows and then for columns
It creates 2D Matrix
Return 1 when failed
Return 0 when succesful
*/
int bitmap_ctor(struct Image *image){

image->colors = (char**)malloc(image->number_of_lines*sizeof(char*));
// Allocation of memory for rows

if (image->colors==NULL){

    fprintf(stderr,"Memory allocation failed! \n");
    return 1;

}// Checks if malloc was successfull

for (int row = 0; row < image->number_of_lines; row++)
{
    image->colors[row] = (char*)malloc(image->number_of_columns*sizeof(char));

    if (image->colors[row]==NULL){

    fprintf(stderr,"Memory allocation failed! \n");
    return 1;

    }// Checks if malloc was successfull

}// Allocation of memory for columns

return 0;

}


/*
This fuction frees memory for image using free
First time for colums and then for rows
*/
void bitmap_dtor(struct Image *image){

for (int row = 0; row < image->number_of_lines; row++)
{
    free(image->colors[row]);
}// Free memory for columns


    free(image->colors); 
    // Free memory for rows
    image->colors=NULL;
    image->number_of_lines=0;
    image->number_of_columns=0;

}


int test_image(struct Image *image, int row, int column){

int is_valid = 1;

if (image->number_of_columns <=0 || image->number_of_lines <= 0 )
{
   is_valid = 0;
}// check if size of image is corect



if ((image->colors[row][column] != '1') && (image->colors[row][column] != '0'))
{
    is_valid = 0;    
}// Check if matrix only contains 0 or 1


if(!is_valid)
{
  fprintf(stdout,"Invalid \n");
}

return is_valid;


}


void validate_image(struct Image *image){


for (int row = 0; row < image->number_of_lines; row++)
{
    for (int column = 0; column < image->number_of_columns; column++)
    {
        if (!test_image(image, row, column))
        {
            return;
        }
         
    }
    
}


fprintf(stdout, "Valid\n");


}


/*
Function for loading image from file
Loads first 2 numbers which are sizes of matrix
Then all numbers in image file
*/
int load_file(struct Image *image, FILE *bitmap){

int row = 0;
int column = 0;
char test_char;
fscanf(bitmap,"%d %d", &image->number_of_lines, &image->number_of_columns);
// Loads first two numbers which are size of image

if (image->number_of_columns <=0 || image->number_of_lines <= 0 )
{
   fprintf(stdout, "Invalid\n");
   return 1;
}// check if size of image is corect




if (bitmap_ctor(image) !=0){
    return 1;
}// Calling function bitmap_ctor, return 1 if fail


for (row = 0; row < image->number_of_lines; row++){

    for (column = 0; column < image->number_of_columns; column++)
    {
        fscanf(bitmap, " %c", &image->colors[row][column]);
    }
    
}



if (fscanf(bitmap, " %c", &test_char) !=  EOF)
{
    fprintf(stdout, "Invalid\n");
    return 1;
}

return 0;
}



void print_help(){




}



void find_hline(struct Image *image){

int longest1 = 0;
int longest2 = -1;
int row_number;
int column_number_start;
int column_number_end;
int off_by_one = 1;

for (int row = 0; row < image->number_of_lines; row++)
{
    for (int column = 0; column < image->number_of_columns; column++)
    {

        if (!test_image(image, row, column))
        {
            return;
        }

        if (image->colors[row][column]=='1')
        {
            longest1++;
        }

        if (image->colors[row][column]=='0')
        {
            if (longest1 > longest2)
            {
                longest2 = longest1;
                row_number = row;
                column_number_start = column - longest2;
                column_number_end = column - off_by_one;

            }
        longest1 = 0;
        }

        if (column == image->number_of_columns - off_by_one)
        {
            if (longest1 > longest2)
            {
                longest2 = longest1;
                row_number = row;
                column_number_start = column - longest2 + off_by_one;
                column_number_end = column;
            }
               
        longest1 = 0; 
        }
        
        
        
        
        

    }

}

if (longest2 > 0)
{
fprintf(stdout,"%d %d %d %d \n", row_number, column_number_start ,
row_number, column_number_end);
}else fprintf(stdout, "No horizontal line found.\n");
}

void find_vline(struct Image *image){
    
int longest1 = 0;
int longest2 = -1;
int column_number;
int row_number_start;
int row_number_end;
int off_by_one = 1;


for (int column = 0; column < image->number_of_columns; column++)
{
    for (int row = 0; row < image->number_of_lines; row++)
    {
        if (!test_image(image, row, column))
        {
            return;
        }

        if (image->colors[row][column] == '1')
        {
            longest1++;
        }

        if (image->colors[row][column]=='0')
        {
            if ((longest1 > longest2) || (longest1 == longest2 && row - longest2 < row_number_start))
            {
                longest2 = longest1;
                column_number = column;
                row_number_start = row - longest2;
                row_number_end = row - off_by_one;
            }


        longest1 = 0;

        } 
        if (row == image->number_of_lines - off_by_one)
        {
            if (longest1 > longest2)
            {
                longest2 = longest1;
                column_number = column;
                row_number_start = row - longest2 + off_by_one;
                row_number_end = row;
            }
            
        
        longest1 = 0;
        }
        


    }

}

if (longest2 > 0)
{
fprintf(stdout,"%d %d %d %d \n",  row_number_start , column_number,
row_number_end, column_number);
}else fprintf(stdout, "No vertical line found.\n");

}

void find_square(struct Image *image){

int row_number_start, row_number_end, column_number_start, column_number_end;
int square_size = 0;
int largest_square_size = -1;
int counter;


for (int row = 0; row < image->number_of_lines; row++)
{
    for (int column = 0; column < image->number_of_columns; column++)
    {
        if (!test_image(image, row, column))
        {
            return;
        }

        if (image->colors[row][column] == '1')
        {
            for (square_size = 0; (square_size + column < image->number_of_columns) 
                && (square_size + row < image->number_of_lines); square_size++)
            {  
              

                if (image->colors[row + square_size][column] == '1' &&
                    image->colors[row][column + square_size] == '1')
                {
                        
                    for (counter = 0; counter <= square_size 
                    && (row + counter < image->number_of_lines) 
                    && (column + counter < image->number_of_columns) ; counter++)
                    {

                        if ((image->colors[row + square_size][column + counter] == '0') 
                        || (image->colors[row + counter][column + square_size] == '0'))
                        {   
                        
                      
                        break;
                    
                        }else 
                        if ((square_size > largest_square_size) && counter == square_size){

                        largest_square_size = square_size;
                        row_number_start = row;
                        column_number_start = column;    
                        row_number_end = row + largest_square_size;
                        column_number_end =  column + largest_square_size;
                    }

                    }
                }else{
              
                break;
                }

               
            }      

       
                

        }
    }
    
}

if (largest_square_size > -1)
{
    fprintf(stdout, "%d %d %d %d\n", row_number_start, column_number_start,
    row_number_end, column_number_end);
} else fprintf(stdout, "No square found. \n");


}

void use_function(struct Image *image, char *function){


if (strcmp(function, "hline")==0)
{
   find_hline(image);
}else
if (strcmp(function, "vline")==0)
{
   find_vline(image);
}else
if (strcmp(function, "square")==0)
{
   find_square(image);
}else
if (strcmp(function, "test")==0)
{
   validate_image(image);
}

}



int main(int argc, char *argv[]){

struct Image image;


FILE *bitmap=fopen(argv[2], "r");
if (bitmap == NULL)
{
    fprintf(stderr, "Error: Cant open file \n");
    return 1;
}





if (load_file(&image, bitmap) != 0)
{
    fclose(bitmap);
    return 1;
}

if (argc==3)
{
    use_function(&image , argv[1]);    
}

fclose(bitmap);

bitmap_dtor(&image);


return 0;

}