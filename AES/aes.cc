#include <math.h>
#include <iostream>
#include <vector>

// Método que hace XOR entre el mensaje y la clave
void
AddRoundKey (std::vector<std::vector<u_int8_t>>& clave, std::vector<std::vector<u_int8_t>>& mensaje) {

  for (auto i = 0; i < 4; i++)
    for (auto j = 0; j < 4; j++)
      mensaje[i][j] ^= clave[i][j]; 
}

// Método que cambia los valores del mensaje según el valor antiguo del mensaje
// y las posiciones en la cajaS.
void
SubBytes (std::vector<std::vector<u_int8_t>>& mensaje, std::vector<u_int8_t>& cajaS) {
  
  for (auto i = 0; i < 4; i++)
    for (auto j = 0; j < 4; j++)
      mensaje[i][j] = cajaS[mensaje[i][j]];
}

// Método que desplaza los valores de las diferentes filas del mensaje.
void
ShiftRows (std::vector<std::vector<u_int8_t>>& mensaje) {
  
  u_int8_t aux;

  // A la fila 0 no se le aplica swap.
  // swap manual de la fila 1
	aux = mensaje[1][0];
	mensaje[1][0] = mensaje[1][1];
	mensaje[1][1] = mensaje[1][2];
	mensaje[1][2] = mensaje[1][3];
	mensaje[1][3] = aux;

  // swap manual de la fila 2
	aux = mensaje[2][0];
	mensaje[2][0] = mensaje[2][2];
	mensaje[2][2] = aux;
	aux = mensaje[2][1];
	mensaje[2][1] = mensaje[2][3];
	mensaje[2][3] = aux;

  // swap manual de la fila 3
	aux = mensaje[3][0];
	mensaje[3][0] = mensaje[3][3];
	mensaje[3][3] = mensaje[3][2];
	mensaje[3][2] = mensaje[3][1];
	mensaje[3][1] = aux;
}

// Método que mezcla las columnas.
void
MixColumns (std::vector<std::vector<u_int8_t>>& mensaje) {
	
  //Variables auxiliares
	u_int8_t a[4];
	u_int8_t b[4];
	u_int8_t h;

  // Con las j preparamos las columnas para hacer XOR y con la i hacemos el XOR.
	for (auto i = 0; i < 4; i++){
		for (auto j = 0; j < 4; j++){
			
			//Copiamos en a el texto cifrado
			a[j] = mensaje[j][i];

			//Multiplicacion entre el byte del texto cifrado y 128 para comprobar si el byte del texto es mayor que 128
			h = mensaje[j][i] & 0x80; 

			//Multiplicamos por 2 (desplazamos un byte) el byte del texto cifrado y lo dejamos en la variable auxiliar b
			b[j] = mensaje[j][i] << 1;

			//En caso de que el byte sea mayor de 127 aplicamos una XOR con 27
			if(h == 0x80)
				b[j] = b[j] ^0x1b;
		}

		//Realizamos todas el XOR.
		mensaje[0][i] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
		mensaje[1][i] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
		mensaje[2][i] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3];
		mensaje[3][i] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0];
	}
}

void
ExpandirClave (std::vector<std::vector<u_int8_t>>& clave, std::vector<u_int8_t>& rCon, 
               std::vector<u_int8_t>& cajaS, unsigned iteracion) {
  
  // RotWord
  // Creamos una columna auxiliar.
  std::vector<u_int8_t> auxColumna = {clave[1][3], clave[2][3], clave[3][3], clave[0][3]};
  
  // SubBytes
  for (auto i = 0; i < 4; i++)
    auxColumna[i] = cajaS[auxColumna[i]];

  // Rcon(8)
  for (auto j = 0; j < 4; j++) {
    for (auto i = 0; i < 4; i++) {
      
      // La primera columna
      if (j == 0) {
        // La primera fila
        if (i == 0) clave[i][j] ^= auxColumna[i] ^ rCon[iteracion];
        // El resto de filas
        else clave[i][j] ^= auxColumna[i];
      }
      // Hacemos XOR con la columna anterior.
      else clave[i][j] ^= clave[i][j - 1];
    }
  }
}

// Método que convierte 2 valore hexadecimales en un entero sin signo de 8 bits.
u_int8_t
SacarNumeroHexa (char& a, char& b) {
    
  u_int8_t pivote = 0;

  // Miramos el valor de b.
  switch (b)
    {
    case 'a':
      pivote += 10 * pow(16, 0);
      break;
    case 'b':
      pivote += 11  * pow(16, 0);
      break;
    case 'c':
      pivote += 12 * pow(16, 0);
      break;
    case 'd':
      pivote += 13 * pow(16, 0);
      break;
    case 'e':
      pivote += 14 * pow(16, 0);
      break;
    case 'f':
      pivote += 15 * pow(16, 0);
      break;
    default:
      pivote += (b - '0') * pow(16, 0);
      break;
    }

  // Miramos el valor de a.
  switch (a)
    {
    case 'a':
      pivote += 10 * pow(16, 1);
      break;
    case 'b':
      pivote += 11  * pow(16, 1);
      break;
    case 'c':
      pivote += 12 * pow(16, 1);
      break;
    case 'd':
      pivote += 13 * pow(16, 1);
      break;
    case 'e':
      pivote += 14 * pow(16, 1);
      break;
    case 'f':
      pivote += 15 * pow(16, 1);
      break;
    default:
      pivote += (a - '0') * pow(16, 1);
      break;
    }

    return pivote;
}

// Método que pasa los valores en hexadecimal de un string a un vector de 
// vectores de enteros sin signo de 8 bits.
std::vector<std::vector<u_int8_t>>
TextoNumero (std::string cadena) {

  // Damos tamaño al vector de vectores
  std::vector<std::vector<u_int8_t>> numeros;
  numeros.resize(4);
  for (auto i = 0; i < 4; i++) {
    numeros[i].resize(4);
  }

  unsigned movimiento = 0;

  // Recorremos el string de 2 en 2 y pasamos los valores al vector.
  for (auto j = 0; j < 4; j++)
    for (auto i = 0; i < 4; i++) {
      numeros[i][j] = SacarNumeroHexa(cadena[movimiento], cadena[movimiento + 1]);
      movimiento++;
      movimiento++;
    }

  return numeros;
}

// Método para pasar a Hexadecimal.
void
PasarHexadecimal(u_int8_t numero) {

    std::string hexa;

    // Dividimos el numero para pasarlo a hexa
    while (numero != 0) {
      switch (numero % 16)
      {
      case 15:
        hexa += 'f';
        break;
      case 14:
        hexa += 'e';
        break;
      case 13:
        hexa += 'd';
        break;
      case 12:
        hexa += 'c';
        break;
      case 11:
        hexa += 'b';
        break;
      case 10:
        hexa += 'a';
        break;                                        
      
      default:
        hexa += numero % 16 + '0';
        break;
      }
       numero /= 16;
    }
    // Rellenamos hasta que sean 2 valores.
    while (hexa.size() != 2)
        hexa += "0";

    // Mostramos por pantalla.
    for (int recorrido = hexa.size() - 1; recorrido >= 0; recorrido--)
        std::cout << hexa[recorrido];
}

void ImprimirVector(std::vector<std::vector<u_int8_t>>& vector) {
  
  // Recorremos el vector y lo imprimimos.
  for (auto j = 0; j < 4; j++)
    for (auto i = 0; i < 4; i++)
      PasarHexadecimal(vector[i][j]);
}

int main(void) {

  std::string lectura;

  std::vector<u_int8_t> 
    Rcon  {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36},
    cajaS {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
           0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
           0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
           0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
           0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
           0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
           0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
           0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
           0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
           0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
           0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
           0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
           0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
           0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
           0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
           0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16};

  std::vector<std::vector<u_int8_t>> clave, mensaje;

  // Leemos de teclado
  std::cout << "Clave: ";
  std::cin >> lectura;
  clave = TextoNumero(lectura);
  //clave = TextoNumero("000102030405060708090a0b0c0d0e0f");

  std::cout << "Bloque de texto original: ";
  std::cin >> lectura;
  mensaje = TextoNumero(lectura);
  std::cout << std::endl;
  //mensaje = TextoNumero("00112233445566778899aabbccddeeff");

  // Iteración 0.
  AddRoundKey(clave, mensaje);
  
  // Mostramos por pantalla.
  std::cout << "R0" << " (Subclave = ";
  ImprimirVector(clave);
  std::cout << ") = ";
  ImprimirVector(mensaje);
  std::cout << std::endl;

  // Iteración 1-9.
  for (auto iteraciones = 1; iteraciones < 10; iteraciones++) {

    SubBytes(mensaje, cajaS);
    ShiftRows(mensaje);
    MixColumns(mensaje);
    ExpandirClave(clave, Rcon, cajaS, iteraciones - 1);
    AddRoundKey(clave, mensaje);

    // Mostramos por pantalla.
    std::cout << "R" << iteraciones << " (Subclave = ";
    ImprimirVector(clave);
    std::cout << ") = ";
    ImprimirVector(mensaje);
    std::cout << std::endl;
  }

  // Iteración 10.
  SubBytes(mensaje, cajaS);
  ShiftRows(mensaje);
  ExpandirClave(clave, Rcon, cajaS, 9);
  AddRoundKey(clave, mensaje);

  // Mostramos por pantalla.
  std::cout << "R10 (Subclave = ";
  ImprimirVector(clave);
  std::cout << ") = ";
  ImprimirVector(mensaje);
  std::cout << std::endl << std::endl;

  std::cout << "Bloque de texto cifrado: ";
  ImprimirVector(mensaje);
  std::cout << std::endl;
}