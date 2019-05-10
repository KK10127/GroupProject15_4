/***********************************************************************
 * Course:CS 151 		Days & Time: Thursday 5:30-9:20
 *
 * Chapter Number:		Project Number:		or GroupProject Name:
 * Programmer:Jay Montoya
 * Date Created:		or	Date Modified:
 *
 * Program Title:
 * Program Description:
 *
 * Algorithm:
 ****************************************************************************/


// This program demonstrates an application of pure virtual functions.
#include <iostream>
#include <fstream>
#include<string>
#include <cstdlib>
using namespace std;

class Encryption {
protected:
    ifstream inFile;
    ofstream outFile;
    int key;
public:
    Encryption(const string& inFileName,
            const string& outFileName, int encryptionKey);

    void setEncryptionKey(int newKey)  { key = newKey; }

    // virtual functions
    virtual ~Encryption();

    virtual char transformTo(char ch) = 0;
    virtual char transformFrom(char ch) = 0;

    virtual void encrypt();
    virtual void decrypt();
};

// Constructor opens the input and output file
Encryption::Encryption(const string &inFileName, const string &outFileName, int encryptionKey) {
    inFile.open(inFileName);
    outFile.open(outFileName);
    key = encryptionKey;
    if(!inFile) {
        cout << "The file " << inFileName
            << " cannot be opened!";
        exit(1);
    }
    if (!outFile) {
        cout << "The file " << outFileName
            << " cannot be opened!";
        exit(1);
    }
}

// Destructor closes the files.
Encryption::~Encryption() {

}

// Encryption function uses the viritual transform member functions
// to transform individual characters.
void Encryption::encrypt() {
    char ch;
    char transCh;
    inFile.get(ch);
    while(!inFile.fail()) {
        transCh = transformTo(ch);
        outFile.put(transCh);
        inFile.get(ch);
    }
    inFile.close();
    outFile.close();
}

void Encryption::decrypt() {
    char ch;
    char transCh;
    inFile.get(ch);
    while(!inFile.fail()) {
        transCh = transformFrom(ch);
        outFile.put(transCh);
        inFile.get(ch);
    }
    inFile.close();
    outFile.close();
}


// The subclass simply overides the virtual transformation function
class SimpleEncryption : public Encryption {
public:
    char transformTo(char ch) {
        int value;
        int wrapAround = 0;

        // modify the key
        setEncryptionKey(key % 128);

        // add the key to the ch and set to value
        value = ch + key;

        // if the value now goes beyond 127
        if (value > 127) {
            // get how much we spilled over the ascii table
            wrapAround = (value % 128);

            // set the ch to the right ascii value
            value = wrapAround - 1;

        }

        // by this point value WILL fit into ch
        ch = value;

        return ch;
    }

    char transformFrom(char ch) {

        int value;
        int wrapAround = 0;

        // modify the key
        setEncryptionKey(key % 128);

        // subtract the key from the ch and put in value
        value = ch - key;

        // if the value now spilled behind 0
        if (value < 0) {
            // get how much spillage there is
            wrapAround = (value % 128);

            // set the ch to the right ascii value
            value = 128 - wrapAround;
        }

        // by this point value WILL fit into ch
        ch = value;

        return ch;
    }

    SimpleEncryption(const string& inFileName,
            const string& outFileName, int encryptionKey) : Encryption(inFileName, outFileName, encryptionKey){

    }


};

int main() {
    string inFileName, outFileName;
    string inFileName2, outFileName2;
    cout << "Enter the name of the file to encrypt: ";
    cin >> inFileName;
    cout << "Enter the name of the file to recieve: ";
    cin >> outFileName;
    SimpleEncryption obfuscate(inFileName, outFileName, 5);
    cout << "\n\tENCRYPTING with a key of 5!" << endl;
    obfuscate.encrypt();


    cout << "\nTime to decrypt!\n\n";
    cout << "Enter the name of the file to decrypt: ";
    cin >> inFileName2;
    cout << "Enter the name of the file to recieve: ";
    cin >> outFileName2;
    SimpleEncryption deobfuscate(inFileName2, outFileName2, 5);
    cout << "\n\tDECRYPTING with a key of 5!" << endl;
    deobfuscate.decrypt();

    cout << "\n all done!";


    return 0;
}