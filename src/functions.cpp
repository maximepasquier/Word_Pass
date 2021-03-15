#include <string.h>
#include <iostream>
#include <fstream>
#include <vector> 
#include <map>
#include <sys/stat.h>
#include <iomanip>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/err.h>

using namespace std;

void handleErrors(void)
{
	ERR_print_errors_fp(stderr);
	abort();
}

string sha256(const string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

int encrypt(unsigned char *password_plaintext, int plaintext_len, unsigned char *clef, unsigned char *iv, unsigned char *ciphertext)
{
	EVP_CIPHER_CTX *ctx;
	int len;
	int ciphertext_len;
	if (!(ctx = EVP_CIPHER_CTX_new())) // crée et initialise le contexte
	{
		handleErrors();
	}
	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, clef, iv)) // un problème a eu lieu
	{
		handleErrors();
	}
	if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, password_plaintext, plaintext_len))
	{
		handleErrors();
	}
	ciphertext_len = len;
	if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
	{
		handleErrors();
	}
	ciphertext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	
	return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *clef, unsigned char *iv, unsigned char *plaintext)
{
	EVP_CIPHER_CTX *ctx;
	int len;
	int plaintext_len;
	if(!(ctx = EVP_CIPHER_CTX_new()))
	{
		handleErrors();
	}
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, clef, iv))
	{
		handleErrors();
	}
	if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
	{
		handleErrors();
	}
	plaintext_len = len;
	if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
	{
		handleErrors();
	}
	plaintext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	
	return plaintext_len;
}

void write(unsigned char* ciphertext, unsigned char* Write_ciphertext)
{
	ofstream binOutFile_Handler;
    ifstream binInFile_Handler;

    // File Open
    binOutFile_Handler.open("./data/Ciphers.dat", ios::out | ios::binary);

    // Check if the file has opened
    if(!binOutFile_Handler)
    {
        cout << "File did not open!";
        exit(1);
    }
    
	memcpy(Write_ciphertext, ciphertext, 128);
	binOutFile_Handler.write((char *) Write_ciphertext, 128);
    binOutFile_Handler.close();
    if(!binOutFile_Handler.good())
    {
        cout << "Error occured during writing the binary file!" << endl;
        exit(2);
    }
	cout << "Ecriture faite !" << endl;
	return;
}

void write_ciphertext_len(int ciphertext_len)
{
	ofstream file;
	file.open("./data/cipher_length.txt");
	file << ciphertext_len;
	file.close();
	return;
}

void write_mdp_id(map<string,string> mdp)
{
	ofstream file;
	file.open("./data/mdp_id.txt");
	for (auto &item : mdp) 
		{
        	file << item.first;
			file << '\n';
    	}
	file.close();
	return;
}

void add_mdp_id(map<string,string> mdp)
{
	ofstream file;
	file.open("./data/mdp_id.txt",ios_base::app);
	for (auto &item : mdp) 
		{
        	file << item.first;
			file << '\n';
    	}
	file.close();
	return;
}

bool File_exists (const string& name)
{
	struct stat buffer;   
	return (stat (name.c_str(), &buffer) == 0); 
}

bool Empty_file(ifstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}

bool read(unsigned char* Read_ciphertext)
{
	ofstream binOutFile_Handler;
    ifstream binInFile_Handler;

	// Vérifier si le fichier existe, sinon le créer
	if(!File_exists("./data/Ciphers.dat")) // return 0 si le file n'existe pas, return 1 si il existe
	{
		binOutFile_Handler.open("./data/Ciphers.dat", ios::out | ios::binary); // crée le fichier car il est non existant
		return true;
	}
	// Vérifier si le fichier est vide
	
	ifstream file("./data/Ciphers.dat");
	if(Empty_file(file))
	{
		cout << "Le fichier est vide!" <<endl;
		return true;
	}

	binInFile_Handler.open("./data/Ciphers.dat", ios::out | ios::binary);

    // Check if the file has opened
    if(!binInFile_Handler)
    {
        cout << "File did not open!" << endl;
        exit(3);
    }
    binInFile_Handler.read((char *) Read_ciphertext, 128);
    binInFile_Handler.close();

    if(!binInFile_Handler.good())
    {
        cout << "Error occured during reading the binary file!" << endl;
        exit(4);
    }
	cout << "Lecture faite !" << endl;
	return false;
}

int read_ciphertext_len()
{
	int ciphertext_len;
	ifstream file;
	file.open("./data/cipher_length.txt");
	file >> ciphertext_len;
	file.close();
	cout << ciphertext_len << endl;
	return ciphertext_len;
}

string convertToString(char* a, int size) 
{ 
    int i; 
    string s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + a[i]; 
    } 
    return s; 
} 

void afficher_plaintext(string plaintext_affichage)
{
	string mot = "";
	string ligne;
	vector<string> clef {};
	vector<string> valeur {};
	ifstream file ("./data/mdp_id.txt");
	if (file.is_open())
	{
		while (getline(file,ligne))
		{
			cout << ligne << endl;
			clef.push_back(ligne);
		}
		file.close();		
	}
	else
	{
		cout << "Impossible d'ouvrir le fichier!" << endl;
	}

	for (auto x : plaintext_affichage)
	{
		if(x == ' ')
		{
			cout << mot << endl;
			valeur.push_back(mot);
			mot = "";
		}
		else
		{
			mot = mot + x;
		}
	}
	cout << mot << endl;
	valeur.push_back(mot);

	for (int i = 0; i < clef.size(); i++)
	{
		cout << clef[i] << " : " << valeur[i] << endl;
	}
	
}
