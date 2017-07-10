#include<vector>

using namespace std;

//set to 1 to print output
//set to >1 to print all output
//set to 0 to print minimal progress messages.
int VERBOSITY = 0;

//CODE BY Theodore Koblesky
//This code uses picopng.cpp which was NOT written by me (see other file in dir)
//This code takes a particular png image file from a oscilloscope screenshot
//and creates a histogram object with the bin contents.
//it relies on the specific RGB combination of teal in the screenshots
void decode_png(char * pngfile, char * histofile)
{  

  gSystem->Load("picopng_cpp.so");

  vector<unsigned char> buffer, image;
  loadFile(buffer, pngfile);
  
  unsigned long image_height, image_width;

  int error = decodePNG(image,image_width,image_height,buffer.empty() ? 0 : &buffer[0],(unsigned long)buffer.size());


  //if there's an error, display it
  if(error != 0) std::cout << "error: " << error << std::endl;
  
  //the pixels are now in the vector "image"

  cout<<"IMAGE SUCESSFULLY IMPORTED"<<endl;
  
  if(VERBOSITY>0)
  {
    if(image.size() > 4) cout << "width: " << image_width << " height: " << image_height << " first pixel: " << std::hex << int(image[0]) << int(image[1]) << int(image[2]) << int(image[3]) << endl;
    cout<<"width: " << image_width << " height: " << image_height<<endl;
  }

  int teal_R = 32;
  int teal_G = 228;
  int teal_B = 224;
  int teal_RGB = teal_R*1000000+teal_G*1000+teal_B;
  
  int teal2_R = 52;
  int teal2_G = 227;
  int teal2_B = 223;
  int teal2_RGB = teal2_R*1000000+teal2_G*1000+teal2_B;

  bool teal_bin = false;
  bool old_teal_bin = false;
  int pixel_RGB = 0;
  bool bin_found = false;
  int first_bin, last_bin = 0;
  int x_bin = 5*image_width;

  //first find the first of the histogram
  for(int y = 0; y < image_height; y++)
  {
    for(int x = 0; x < image_width; x++)
    {
      if(VERBOSITY>1)
        cout<<"RGBA for pixel: "<<y*image_height+x<<endl;
      for(int ibit = 0; ibit<3; ibit++)
      {
       int i = (y * image_height + x)*4 + ibit;

       if(ibit==0) pixel_RGB+=image[i]*1000000;
       if(ibit==1) pixel_RGB+=image[i]*1000;
       if(ibit==2) pixel_RGB+=image[i];

       if(VERBOSITY>1)
         cout<<int(image[i])<<endl;
     }

     if(teal_RGB==pixel_RGB || teal2_RGB==pixel_RGB)
     {
       teal_bin = true;
     }
     else 
       teal_bin = false;

     if(teal_bin && !old_teal_bin) {first_bin = y; bin_found = true; break;}
     old_teal_bin = teal_bin;
     pixel_RGB = 0;
   }
   if(bin_found) break;
 }

 teal_bin = false;
 old_teal_bin = false;
 bin_found = false;
 pixel_RGB = 0;
  //then find the last of the histogram
 for(int y = image_height-1; y >= 0; y--)
 {
  for(int x = 0; x < image_width; x++)
  {
    if(VERBOSITY>1)
      cout<<"RGBA for pixel: "<<y*image_height+x<<endl;
    for(int ibit = 0; ibit<3; ibit++)
    {
     int i = (y * image_height + x)*4 + ibit;

     if(ibit==0) pixel_RGB+=image[i]*1000000;
     if(ibit==1) pixel_RGB+=image[i]*1000;
     if(ibit==2) pixel_RGB+=image[i];

	  if(VERBOSITY>1)
      cout<<int(image[i])<<endl;
   }

   if(teal_RGB==pixel_RGB || teal2_RGB==pixel_RGB)
   {
     teal_bin = true;
   }
   else 
     teal_bin = false;

   if(teal_bin && !old_teal_bin) {last_bin = y; bin_found = true; break;}
   old_teal_bin = teal_bin;
   pixel_RGB = 0;
 }
 if(bin_found) break;
}

if(VERBOSITY)
  cout<<"first_bin: "<<first_bin<<" last_bin: "<<last_bin<<endl;

if(first_bin >= last_bin) {cout<<"error! first bin > last bin"<<endl; return;}

teal_bin = false;
old_teal_bin = false;
pixel_RGB = 0;

TFile *file = TFile::Open(histofile,"RECREATE");

TH1I *scope_hist = new TH1I("scope_hist","scope_hist",last_bin-first_bin,-0.5,last_bin-first_bin-0.5);

//then make the histogram
for(int y = first_bin; y < last_bin+1; y++)
{
  for(int x = 0; x < image_width; x++)
  {
    if(VERBOSITY>1)
      cout<<"RGBA for pixel: "<<y*image_height+x<<endl;
    for(int ibit = 0; ibit<4; ibit++)
    {
     int i = (y * image_height + x)*4 + ibit;

     if(ibit==0) pixel_RGB+=image[i]*1000000;
     if(ibit==1) pixel_RGB+=image[i]*1000;
     if(ibit==2) pixel_RGB+=image[i];

   }

   if(teal_RGB==pixel_RGB || teal2_RGB==pixel_RGB)
   {
     scope_hist->SetBinContent(y - first_bin, scope_hist->GetBinContent(y-first_bin)+1);
   }
   pixel_RGB = 0;
 }
}

 cout<<"HISTOGRAME CREATED"<<endl;

scope_hist->Write();
file->Close();
return;
}
