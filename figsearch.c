#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    
// Allocation of memory for rows
image->colors = (char**)malloc(image->number_of_lines*sizeof(char*));

// Checks if malloc was successfull
if (image->colors==NULL){

    fprintf(stderr,"Memory allocation failed! \n");

    return 1;

}

// Allocation of memory for columns
for (int row = 0; row < image->number_of_lines; row++)
{
    image->colors[row] = (char*)malloc(image->number_of_columns*sizeof(char));

    // Checks if malloc was successfull if not free all mallocs
    if (image->colors[row]==NULL){

        fprintf(stderr,"Memory allocation failed! \n");

        for (int i = 0; i < row; i++)
        {
            free(image->colors[i]);
        }

        free(image->colors);
        return 1;

    }

}

return 0;

}


/*
This fuction frees memory for image using free
First time for colums and then for rows
*/
void bitmap_dtor(struct Image *image){

// Free memory for columns
for (int row = 0; row < image->number_of_lines; row++)
{
    free(image->colors[row]);
}

 // Free memory for rows
free(image->colors);   
image->colors=NULL;
image->number_of_lines=0;
image->number_of_columns=0;

}


/*
This fuction test file while program is running
Returns 1 if file is valid
Returns 0 if file is invalid and prints Invalid on stderr
*/

int test_image(struct Image *image, int row, int column){

int is_valid = 1; //expecting that file is valid

// check if size of image is corect
if (image->number_of_columns <=0 || image->number_of_lines <= 0 )
{
    is_valid = 0;
}


// Check if matrix only contains 0 or 1
if ((image->colors[row][column] != '1') && (image->colors[row][column] != '0'))
{
    is_valid = 0;    
}


if(!is_valid)
{
    fprintf(stderr,"Invalid\n");
}

return is_valid;


}

/*
Uses funcion test_image() for testing whole file using two for loops
Print Invalid when file is invalid on stderr
Print Valid when file is valid on stdout
*/

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
Return 0 when loading was succesful
Return 1 when loading fail
*/
int load_file(struct Image *image, FILE *bitmap){

int row = 0;
int column = 0;

// Loads first two numbers which are size of image
fscanf(bitmap,"%d %d", &image->number_of_lines, &image->number_of_columns);


// check if size of image is corect
if (image->number_of_columns <=0 || image->number_of_lines <= 0 )
{
   fprintf(stderr, "Invalid\n");
   return 1;
}



// Calling function bitmap_ctor, return 1 if fail
if (bitmap_ctor(image) !=0){
    return 1;
}

//Scan file and indexing each symbol
for (row = 0; row < image->number_of_lines; row++){

    for (column = 0; column < image->number_of_columns; column++)
    {
        fscanf(bitmap, " %c", &image->colors[row][column]);
    }
    
}

return 0;
}


// This function prints help for using program on stdout
void print_help(){

fprintf(stdout,"The program starts in the following form:\n"
"./fisearch --help\nor\n./fisearch \"argument\" FILE\n\n"
"\"--help\" will cause the program to print a help for using program and exit.\n\n"
"\"test\" only checks that the file given by the second argument of the program contains\n"
"a proper definition of the bitmap image. If the image format matches the definition, \n"
"it prints \"Valid\" (on stdout). Otherwise (e.g. illegal characters, missing data or \n"
"wrong values) the program prints \"Invalid\" (on stderr). \n\n"
"\"hline\" finds and prints the start and end coordinates of the first longest horizontal\n"
"line in the given image.\n\n"
"\"vline\" finds and prints the start and end coordinates of the first longest vertical\n"
"segment in the given image.\n\n"
"\"square\" finds and prints the start and end coordinates of the first largest square\n"
"in the given image.\n\n"
"FILE represents the name of the file containing the bitmap content.\n");


}



/*
This function finds longest horizontal line
Prints start and end coordinates if found else prints Not found on stdout
Start coordinates are closest to the left
*/
void find_hline(struct Image *image){

int longest1 = 0;
int longest2 = 0;
int row_number; // only one number needed cause row coordinates are same for each line
int column_number_start;
int column_number_end;
int off_by_one = 1; //for correcting off by one errors

//go by each row
for (int row = 0; row < image->number_of_lines; row++)
{   
    //go by each column
    for (int column = 0; column < image->number_of_columns; column++)
    {

        //testing image if it is right format
        if (!test_image(image, row, column))
        {
            return;
        }

        // if character is equal to 1 add to longest1
        if (image->colors[row][column]=='1')
        {
            longest1++;
        }

        //if character is equal to 0 check if line 1 is logner than line 2
        if (image->colors[row][column]=='0')
        {
            // if new line is longer rewrite coordinates
            if (longest1 > longest2) 
            {
                longest2 = longest1;
                row_number = row;
                column_number_start = column - longest2;
                // of by one cause at next charater that is not 1
                column_number_end = column - off_by_one; 
                

            }
        //reset counter
        longest1 = 0;
        }

        //if it is last column index check if line 1 is longer than line 2
        if (column == image->number_of_columns - off_by_one)
        {
            // if new line is longer rewrite coordinates
            if (longest1 > longest2)
            {
                longest2 = longest1;
                row_number = row;
                //of by one cause i am at the end of line of columns
                column_number_start = column - longest2 + off_by_one;
                column_number_end = column;
            }
        //reset counter       
        longest1 = 0; 
        }
        
    }

}

//print coordinates if line exists else print Not found
if (longest2 > 0)
{
fprintf(stdout,"%d %d %d %d \n", row_number, column_number_start ,
row_number, column_number_end);
}else fprintf(stdout, "Not found\n");


}

/*
This function finds longest vertical line
Prints start and end coordinates if found else prints Not found on stdout
Start coordinates are closest to the left
*/

void find_vline(struct Image *image){
    
int longest1 = 0;
int longest2 = 0;
int column_number; //only needed one number cause line is on same column line
int row_number_start;
int row_number_end;
int off_by_one = 1; //for correcting off by one errors

//go by each column
for (int column = 0; column < image->number_of_columns; column++)
{
    //go by each row
    for (int row = 0; row < image->number_of_lines; row++)
    {
        //testing image if it is right format
        if (!test_image(image, row, column))
        {
            return;
        }

        // if character is equal to 1 add to longest1
        if (image->colors[row][column] == '1')
        {
            longest1++;
        }

        //if character is equal to 0 check if line 1 is logner than line 2
        if (image->colors[row][column]=='0')
        {
            // if new line is longer or is uqual but row index is smaller rewrite coordinates
            if ((longest1 > longest2) 
                || (longest1 == longest2 && row - longest2 < row_number_start))
            {
                longest2 = longest1;
                column_number = column;
                row_number_start = row - longest2;
                // of by one cause at next charater that is not 1
                row_number_end = row - off_by_one;
            }

            //reset counter
            longest1 = 0;

        } 

        //if it is last row index check if line 1 is longer than line 2
        if (row == image->number_of_lines - off_by_one)
        {
            // if new line is longer rewrite coordinates
            if (longest1 > longest2)
            {
                longest2 = longest1;
                column_number = column;
                //of by one cause i am at the end of line of rows
                row_number_start = row - longest2 + off_by_one;
                row_number_end = row;
            }
            
            //reset counter
            longest1 = 0;
        }

    }

}

//print coordinates if line exists else print Not found
if (longest2 > 0)
{
    fprintf(stdout,"%d %d %d %d \n",  row_number_start , column_number,
    row_number_end, column_number);
}else 
    fprintf(stdout, "Not found\n");

}

/*
This function finds largest square
Prints start and end coordinates if found else prints Not found on stdout
Start coordinates are closest to the left
*/
void find_square(struct Image *image){

int row_number_start, row_number_end, column_number_start, column_number_end;
int square_size = 0;
int largest_square_size = -1; // set to -1 cause square can be size 0
int counter;

//go by rows
for (int row = 0; row < image->number_of_lines; row++)
{
    //go by columns
    for (int column = 0; column < image->number_of_columns; column++)
    {
        //testing image if it is right format
        if (!test_image(image, row, column))
        {
            return;
        }


        //if character is equal to 1 check for each square that could go from this point   
        if (image->colors[row][column] == '1')
        {
            //go by each square size that could be without going out of bounds
            for (square_size = 0; (square_size + column < image->number_of_columns) 
                && (square_size + row < image->number_of_lines); square_size++)
            {  
                //if neighbouring columns also contains 1 continue else break
                if (image->colors[row + square_size][column] == '1' &&
                    image->colors[row][column + square_size] == '1')
                {
                    // go by each parallel position that is in square without going out of bounds
                    for (counter = 0; counter <= square_size 
                    && (row + counter < image->number_of_lines) 
                    && (column + counter < image->number_of_columns) ; counter++)
                    {
                        //if parallel position equals to zero break
                        if ((image->colors[row + square_size][column + counter] == '0') 
                        || (image->colors[row + counter][column + square_size] == '0'))
                        {   

                            break;
                    
                        }else
                        /*
                        if size of new square is bigger that previos one
                        and counter is at the end write new coordinates
                        */ 
                        if ((square_size > largest_square_size) && counter == square_size){

                            largest_square_size = square_size;
                            row_number_start = row;
                            column_number_start = column;    
                            row_number_end = row + largest_square_size;
                            column_number_end =  column + largest_square_size;
                        }

                    }
                }else break;
                    

               
            }      

        }

    }
    
}

//if square exist print coordinates on stdout else print Not found
if (largest_square_size > -1)
{
    fprintf(stdout, "%d %d %d %d\n", row_number_start, column_number_start,
    row_number_end, column_number_end);
} else fprintf(stdout, "Not found\n");


}

/*
This funcion selects right function give by argument
If wrong argument it prints Wrong argument
*/
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
}else
if (strcmp(function, "--help")==0)
{
   print_help();
}else fprintf(stderr,"Wroung argument. Try: ./figsearch --help\n");

}



int main(int argc, char *argv[]){

struct Image image;


if (argc==3)
{
    FILE *bitmap=fopen(argv[2], "r");

    //check if opening file was succesful
    if (bitmap == NULL)
    {
        fprintf(stderr, "Error: Cant open file.\n");
        return 1;
    }

    //if load_file() failed return 1
    if (load_file(&image, bitmap) != 0)
    {
        fclose(bitmap);
        return 1;
    }

    use_function(&image , argv[1]);
    fclose(bitmap);
    bitmap_dtor(&image);

}else
if (argc==2 && strcmp(argv[1], "--help")==0)
{
    print_help();
}else 
fprintf(stderr,"Wrong argument. Try: ./figsearch --help\n");



return 0;

}