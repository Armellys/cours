# Compilateur C++ utilisé
CC = g++

# Optimisations  
OPTI = -O3 

# Options de compilation
CFLAGS = -c $(OPTI)

# Librairies supplémentaires pour l'édition de liens
LFLAGS = -lm

BIN = bin

# Fichiers sources 
SRCS= dct.cpp attacks.cpp mtrand.cpp 

# Objets
OBJS= $(SRCS:%.cpp=%.o) 


# Chemin d'accès aux sources et aux objets
VPATH= $(BIN)  

# Cibles principales
all: lsb_write lsb_read qt_write qt_read ss_write ss_read jpeg noise scale psnr 


# Edition de liens
lsb_write: $(OBJS) lsb_write.o
		$(CC) $(BIN)/lsb_write.o $(OBJS:%.o=$(BIN)/%.o) -o $(BIN)/lsb_write $(LFLAGS) 
		
lsb_read: $(OBJS) lsb_read.o
		$(CC) $(BIN)/lsb_read.o $(OBJS:%.o=$(BIN)/%.o) -o $(BIN)/lsb_read $(LFLAGS)

qt_write: $(OBJS) qt_write.o
		$(CC) $(BIN)/qt_write.o $(OBJS:%.o=$(BIN)/%.o) -o $(BIN)/qt_write $(LFLAGS)
		
qt_read: $(OBJS) qt_read.o
		$(CC) $(BIN)/qt_read.o $(OBJS:%.o=$(BIN)/%.o) -o $(BIN)/qt_read $(LFLAGS)
		
ss_write: $(OBJS) ss_write.o
		$(CC) $(BIN)/ss_write.o $(OBJS:%.o=$(BIN)/%.o) -o $(BIN)/ss_write $(LFLAGS)
		
ss_read: $(OBJS) ss_read.o
		$(CC) $(BIN)/ss_read.o $(OBJS:%.o=$(BIN)/%.o) -o $(BIN)/ss_read $(LFLAGS)

tardos: $(OBJS) tardos.o
		$(CC) $(BIN)/tardos.o $(OBJS:%.o=$(BIN)/%.o) -o $(BIN)/tardos $(LFLAGS)
		

jpeg: $(OBJS) jpeg.o
		$(CC) $(BIN)/jpeg.o $(OBJS:%.o=$(BIN)/%.o) -o $(BIN)/jpeg $(LFLAGS)
		
noise: $(OBJS) noise.o
		$(CC) $(BIN)/noise.o $(OBJS:%.o=$(BIN)/%.o) -o $(BIN)/noise $(LFLAGS)
				
scale: $(OBJS) scale.o
		$(CC) $(BIN)/scale.o $(OBJS:%.o=$(BIN)/%.o) -o $(BIN)/scale $(LFLAGS)
		
psnr: $(OBJS) psnr.o
		$(CC) $(BIN)/psnr.o $(OBJS:%.o=$(BIN)/%.o) -o $(BIN)/psnr $(LFLAGS)

# Compilation des sources 
%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $(BIN)/$(@F)

depend::
	g++ -MM  $(CFLAGS) `ls **/*.cpp` > .depend


# Pour nettoyer un peu
clean:
	$(RM) $(BIN)/*.o

-include .depend
