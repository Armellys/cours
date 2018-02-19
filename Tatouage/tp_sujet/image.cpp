/*
	A simple image template class - v. 0.5 - Mar. 10
 
	Copyright (c) 2010, Gaëtan Le Guelvouit <gaetan.leguelvouit@orange-ftgroup.com>
 
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License as
	published by the Free Software Foundation; either version 2 of
	the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be
	useful, but WITHOUT ANY WARRANTY; without even the implied
	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
	PURPOSE.  See the GNU General Public License for more details.
 
	You should have received a copy of the GNU General Public
	License along with this program; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
	MA 02111-1307, USA.
*/

template <class T>
image<T>::image(const int n_width, const int n_height) : w(n_width), h(n_height), buffer(NULL)
{
	if(w*h>0) buffer=new T[w*h];
}
	
template <class T>
image<T>::image(const int n_width, const int n_height, const T &val) : w(n_width), h(n_height), buffer(NULL)
{
	if(w*h>0)
	{
		buffer=new T[w*h];
	
		for(int i=0; i<w*h; i++) buffer[i]=val;
	}
}

template <class T>
image<T>::image(const image &img) : w(img.w), h(img.h), buffer(NULL)
{
	if(w*h>0)
	{
		buffer=new T[w*h];
		
		for(int i=0; i<w*h; i++) buffer[i]=img[i];
	}
}

template <class T>	
image<T>::image(const int n_width, const int n_height,  const T* const buf) : w(n_width), h(n_height), buffer(NULL)
{
	for(int i=0; i<w*h; i++) buffer[i]=buf[i];
}

template <class T>
image<T>::~image()
{
	if(buffer!=NULL) delete [] buffer; 
}

template <class T>
image<T>& image<T>::operator=(const image& img)
{
	resize(img.w, img.h);
	
	if(w*h>0)
	{
		for(int i=0; i<w*h; i++) buffer[i]=img[i];
	}
	else buffer=NULL;

	return *this;
}

template <class T>
void image<T>::init(const T &val)
{
	for(int i=0; i<w*h; i++) buffer[i]=val;
}

template <class T>
void image<T>::resize(const int n_width, const int n_height)
{
	if(n_width*n_height!=w*h)
	{
		if(buffer!=NULL) delete [] buffer; // Pose pb ? 
		
		buffer=new T[n_width*n_height];
	}
	
	w=n_width;
	h=n_height;
}


template <class T>
void image<T>::skip_space(istream &is) 
{
  int skipped = 0;

  for(int b=is.peek(); (b==' ')||(b=='\n')||(b==0)||(b=='#')||(b=='\t'); b=is.peek())
  {
    skipped++;
    if(b=='#') // A comment  
    {
      is.ignore(100,'\n');
    }
    else is.get();
	}
}


template <class T>
int image<T>::read(const char* const file) // General case: PGM reading
{
	int W, H, N; 

	// Open file  
	ifstream ifs(file, ios::in|ios::binary);

  if (ifs.fail()) {ERR("Unable to open " << file << "." << endl);}
  
  // Check for P5 type 
  if((ifs.get()!='P')||(ifs.get()!='5')){cerr << "Cannot find P5 pattern." << endl; return 0;}
  
  // Header reading  
  skip_space(ifs); ifs >> W;
  skip_space(ifs); ifs >> H;
  skip_space(ifs); ifs >> N;

  if (ifs.fail() || N>255){ERR("Invalid PGM format." << endl);}

  if(ifs.get()!='\n'){ERR("Invalid PGM format." << endl);}

	unsigned char* b = new unsigned char[W*H];
	
  ifs.read((char*)b, W*H); 

  if(ifs.fail()){delete [] b; ERR("Cannot read from stream." << endl); return -1;}

  resize(W, H);
  for(int i=0; i<w*h; i++) buffer[i]=(T)(b[i]);

  delete [] b;
  return 1;
}
  
template <>
inline int image<rgb>::read(const char* const file) // Perticuliar case: PPM
{
	int W, H, N; 

	// Open file 
	ifstream ifs(file, ios::in|ios::binary);

  if (ifs.fail()) {ERR("Unable to open " << file << "." << endl);}

	// Check for P6 type 
  if((ifs.get()!='P')||(ifs.get()!='6')){ERR("Cannot find P6 pattern." << endl);}
  
  // Header data 
  skip_space(ifs); ifs >> W;
  skip_space(ifs); ifs >> H;
  skip_space(ifs); ifs >> N;
  
  if (ifs.fail() || N>255){ERR("Invalid PPM format." << endl);}

  if(ifs.get()!='\n'){ERR("Invalid PPM format." << endl);}
  
  unsigned char* b = new unsigned char[3*W*H];
  
  ifs.read((char*)b, 3*W*H); 
  
  if(ifs.fail()){delete [] b; ERR("Cannot read from stream." << endl); return -1;}

  resize(W, H);
  for(int i=0; i<w*h; i++)
  {
  	buffer[i].r=(octet)b[3*i + 0];
  	buffer[i].g=(octet)b[3*i + 1];
  	buffer[i].b=(octet)b[3*i + 2];
	}
  
  delete [] b;
  return 1;
}

template <class T>
int image<T>::write(const char* const file) const // General case: PGM 
{
	// Open file 
	ofstream ofs(file, ios::out|ios::binary);
 
  if(ofs.fail()){ERR("Cannot open " << file << "." << endl);}

	// Write header 
  ofs << "P5" << endl << "# Made by the IMAGE template class\n" << w << " " << h << endl << 255 << endl;

	unsigned char* b = new unsigned char[w*h];
  
  for(int i=0; i<w*h; i++) b[i]=(unsigned char)buffer[i];

  ofs.write((char*)b, w*h);
  
  delete [] b;
  
  if(ofs.fail()){ERR("Cannot write in stream." << endl);} else return 1;
}

template <>
inline int image<rgb>::write(const char* const file) const // PPM reading  
{
	// Open file  
	ofstream ofs(file, ios::out|ios::binary);
 
  if(ofs.fail()){ERR("Cannot open " << file << "." << endl);}

  ofs << "P6" << endl << "# Made by the IMAGE template class\n" << w << " " << h << endl << 255 << endl;

	unsigned char *b = new unsigned char[3*w*h];

  for(int i=0, j=0; i<w*h; i++)
  {
    b[j++] = (unsigned char)buffer[i].r;
    b[j++] = (unsigned char)buffer[i].g;
    b[j++] = (unsigned char)buffer[i].b;
  }
 
  ofs.write((char*)b, 3*w*h);

  delete [] b;

  if(ofs.fail()){ERR("Cannot write in stream." << endl);} else return 1;
}

