#include <string.h>
#include <iostream>
#include <map>
#include "functions.h"

using namespace std;

int main(int argc, char const *argv[])
{
	unsigned char *clef;										 // pointeur sur la clef de cryptage
	unsigned char *iv = (unsigned char *)"f7ez4hao92hcne64";	 // 16 char de long, 128 bits de valeur d'initialisation car 16*8
	unsigned char *password_plaintext;							 // pointeur sur le plaintext à encrypter
	unsigned char ciphertext[128];								 // buffer pour le ciphertext
	unsigned char Write_ciphertext[128];						 // buffer permettant le stockage dans la mémoire
	unsigned char Read_ciphertext[128];							 // buffer contenant le ciphertext lu à partir du fichier
	unsigned char plaintext[128];								 // buffer pour le ciphertext déchiffré
	int plaintext_len, ciphertext_len;							 // longueur des deux chaine de char
	bool Fichier_vide;											 // indique si le fichier est vide ou non
	string mot_de_passe;										 // mot de passe principal
	string mot_de_passe_hache;									 // mot de passe principal haché
	int mot_de_passe_hache_length = mot_de_passe_hache.length(); // longueur du mot de passe haché ??????
	string plaintext_affichage;									 // permet l'affichage à l'utilisateur
	string nouveau_mot_de_passe;								 // nouveau mot de passe à enregistrer
	string nouveau_mot_de_passe_id;								 // id du nouveau mot de passe à enregistrer
	string tmp_string;											 // string temporaire, usage multiple
	int nouveau_mot_de_passe_length;							 // ne sert a rien ???
	bool nouvelle_saisie = true;								 // détermine si on effectue un saisie de mot de passe
	map<string, string> mdp;									 // dictionnaire des id et des mots de passe

	//TODO : FONCTION DE PRÉSENTATION DU LOGICIEL !

	//* Demande du mot de passe principal à l'utilisateur

	cout << "Entrez votre mot de passe principal : ";
	getline(cin, mot_de_passe);												  // lire la ligne, stocker dans mot_de_passe
	cin.clear();															  // clear l'entrée
	cout << "Le mot de passe principal entré est : " << mot_de_passe << endl; //
	mot_de_passe_hache = sha256(mot_de_passe);								  // hache le mot de passe
	clef = (unsigned char *)&mot_de_passe_hache[0];							  // pointe sur l'adresse du début du string
	cout << "Le clef est : " << clef << endl;

	//* Lecture + (initialisation si nécessaire) du fichier .dat contenant le ciphertext

	Fichier_vide = read(Read_ciphertext); // true = aucun cipher, false = ciphers à déchiffrer

	//* Le fichier est vide

	if (Fichier_vide)
	{
		cout << "Aucun mot de passe enregistré !" << endl;

		mdp = Saisie_mdp();

		afficher_dictionnaire(mdp);

		nouveau_mot_de_passe.clear();
		nouveau_mot_de_passe = get_dictionary_mdp(mdp, nouveau_mot_de_passe);

		write_mdp_id(mdp); // sauve les id des mots de passe

		password_plaintext = (unsigned char *)&nouveau_mot_de_passe[0];
		cout << "password_plaintext vaut : " << password_plaintext << endl;
		ciphertext_len = encrypt(password_plaintext, strlen((char *)password_plaintext), clef, iv, ciphertext);
		write(ciphertext, Write_ciphertext);
		write_ciphertext_len(ciphertext_len);
	}
	else //* Le fichier contient des ciphertext, on les décrypte
	{
		ciphertext_len = read_ciphertext_len();										   // récupère la longueur du ciphertext
		plaintext_len = decrypt(Read_ciphertext, ciphertext_len, clef, iv, plaintext); // déchiffre le message
		plaintext[plaintext_len] = '\0';											   // marque la fin de la chaine
		printf("Le decrypted text est :\n");										   //
		printf("%s\n", plaintext);													   //
		plaintext_affichage = convertToString((char *)plaintext, plaintext_len);	   // conversion en string
		afficher_plaintext(plaintext_affichage);									   // affiche id et mots de passe

		cout << "Des mot de passe du fichier sont déjà sauvé dans le array plaintext : " << plaintext << endl;
		cout << "Que souhaitez vous faire ? reset/add" << endl;
		getline(cin, tmp_string);
		cin.clear();

		if (tmp_string == "reset")
		{
			cout << "Suppression des mots de passe, veuillez en entrer des nouveaux :" << endl;

			mdp = Saisie_mdp();

			afficher_dictionnaire(mdp);

			nouveau_mot_de_passe.clear();
			nouveau_mot_de_passe = get_dictionary_mdp(mdp, nouveau_mot_de_passe);

			write_mdp_id(mdp); // sauve les id des mots de passe

			password_plaintext = (unsigned char *)&nouveau_mot_de_passe[0];
			cout << "password_plaintext vaut : " << password_plaintext << endl;
			ciphertext_len = encrypt(password_plaintext, strlen((char *)password_plaintext), clef, iv, ciphertext);
			write(ciphertext, Write_ciphertext);
			write_ciphertext_len(ciphertext_len);
		}
		if (tmp_string == "add")
		{
			string plaintext_to_string = convertToString((char *)plaintext, plaintext_len);
			cout << "la version en string est : " << plaintext_to_string << endl;

			mdp = Saisie_mdp();

			afficher_dictionnaire(mdp);

			nouveau_mot_de_passe = get_dictionary_mdp(mdp, plaintext_to_string);

			add_mdp_id(mdp); // sauve les id des mots de passe

			password_plaintext = (unsigned char *)&nouveau_mot_de_passe[0];
			cout << "password_plaintext vaut : " << password_plaintext << endl;
			ciphertext_len = encrypt(password_plaintext, strlen((char *)password_plaintext), clef, iv, ciphertext);
			write(ciphertext, Write_ciphertext);
			write_ciphertext_len(ciphertext_len);
		}
	}
	return 0;
}
