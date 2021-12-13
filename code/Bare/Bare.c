

void main()
{
    /*
    unsigned char v1, v2, v3, v4;

    *(unsigned char*)0x00020000 = 0x11;
    *(unsigned char*)0x00020001 = 0x22;
    *(unsigned char*)0x00020002 = 0x33;
    *(unsigned char*)0x00020003 = 0x44;

    v1 = *(unsigned char*)0x00020000;
    v2 = *(unsigned char*)0x00020001;
    v3 = *(unsigned char*)0x00020002;
    v4 = *(unsigned char*)0x00020003;

    *(unsigned char*)0x00020000 = v4;
    *(unsigned char*)0x00020001 = v3;
    *(unsigned char*)0x00020002 = v2;
    *(unsigned char*)0x00020003 = v1;
    */

   unsigned short v1, v2;

   *(unsigned short*)0x00020000 = 0x1111;
   *(unsigned short*)0x00020002 = 0x2222;

    v1 = *(unsigned short*)0x00020000;
    v2 = *(unsigned short*)0x00020002;   
}