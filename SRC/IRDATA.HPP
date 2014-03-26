#ifndef IRDATA_HPP
#define IRDATA_HPP

#define IRDATA_LEN 6


class IrData
{
public:
    BOOL operator==(const IrData& ird);
    BOOL operator!=(const IrData& ird);
    // assign operator is overloaded because we need to copy memory data
    IrData& operator=(const IrData& ird);
    IrData& operator=(char *strData);
    char *GetpChar();
    char *GetXString();
    IrData(const IrData& ird);
    IrData(char *strData);
    IrData();
    void Print(FILE *f=stdout);
    IrData& Read(FILE *f=stdin);
private:
    // Variabledeclaraties
    char pcData[IRDATA_LEN];
};

#endif
