#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILEPATH "obrazek.txt"

struct Image
{
   int number_of_lines;  // variable that stores number of lines
   int number_of_columns; // variable that stores number of columns
   int **colors; // field of bitmap colors
};


/*
This fuction allocating memory for image using malloc
First time for rows and then for columns
It creates 2D Matrix
Return 1 when failed
Return 0 when succesful
*/
int bitmap_ctor(struct Image *image){

image->colors = (int**)malloc(image->number_of_lines*sizeof(int*));
// Allocation of memory for rows

if (image->colors==NULL){

    fprintf(stderr,"Memory allocation failed! \n");
    return 1;

}// Checks if malloc was successfull

for (int row = 0; row < image->number_of_lines; row++)
{
    image->colors[row] = (int*)malloc(image->number_of_columns*sizeof(int));

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







/*
Function for loading image from file
Loads first 2 numbers which are sizes of matrix
Then all numbers in image file
*/
int load_file(struct Image *image, FILE *bitmap){

fscanf(bitmap,"%d %d", &image->number_of_lines, &image->number_of_columns);
// Loads first two numbers which are size of image

if (bitmap_ctor(image) !=0){
    return 1;
}// Calling function bitmap_ctor, return 1 if fail


for (int row = 0; row < image->number_of_lines; row++){

    for (int column = 0; column < image->number_of_columns; column++)
    {
        fscanf(bitmap,"%d", &image->colors[row][column]);
    }
    
}


return 0;
}



void print_help(){




}

int test(struct Image *image, int row, int column){

int is_valid = 1;

if (image->number_of_columns <=0 || image->number_of_lines <= 0 )
{
   is_valid = 0;
}// check if size of image is corect



if (image->colors[row][column] != 1 && image->colors[row][column] != 0)
{
    is_valid = 0;       
}// Check if matrix only contains 0 or 1


if(is_valid)
{
   fprintf(stdout,"Invalid \n");
}else fprintf(stdout,"Valid \n");

return is_valid;
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
        if (image->colors[row][column]==1)
        {
            longest1++;
        }

        if (image->colors[row][column]==0)
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

fprintf(stdout,"%d %d %d %d \n", row_number, column_number_start ,
row_number, column_number_end);

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
        if (image->colors[row][column] == 1)
        {
            longest1++;
        }

        if (image->colors[row][column]==0)
        {
            if (longest1 > longest2)
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

fprintf(stdout,"%d %d %d %d \n",  row_number_start , column_number, row_number_end, column_number);


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

        if (image->colors[row][column] == 1)
        {
            for (square_size = 0; (square_size + column < image->number_of_columns) 
                && (square_size + row < image->number_of_lines); square_size++)
            {  
              

                if (image->colors[row + square_size][column] == 1 &&
                    image->colors[row][column + square_size] == 1)
                {
                        
                    for (counter = 0; counter <= square_size && (row + counter < image->number_of_lines) &&
                                    (column + counter < image->number_of_columns) ; counter++)
                    {

                        if ((image->colors[row + square_size][column + counter] == 0) 
                        || (image->colors[row + counter][column + square_size] == 0))
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
}
if (strcmp(function, "vline")==0)
{
   find_vline(image);
}
if (strcmp(function, "square")==0)
{
   find_square(image);
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