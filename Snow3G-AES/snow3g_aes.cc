#include <math.h>
#include <iostream>
#include <vector>


// Método que calcula una clave de forma aleatoria
std::vector<u_int8_t>
Aleatorio (unsigned cantidad){

    srand(time(NULL));
    std::vector<u_int8_t> vector;
    vector.resize(cantidad);
    for (unsigned recorrido = 0; recorrido < vector.size(); recorrido++)
        vector[recorrido] = rand()%256; // Valores entre 0 y 255

    return vector;
}

// Método para pasar a binario.
void
PasarBinario(u_int8_t numero) {

  std::string binario;

  // Dividimos el numero para pasarlo a binario
  while (numero != 0) {
    binario += (numero % 2 == 0 ? "0" : "1");
    numero /= 2;
  }
  // Rellenamos hasta que sean 8 valores.
  while (binario.size() != 8)
    binario += "0";

  // Mostramos por pantalla.
  for (int recorrido = binario.size() - 1; recorrido >= 0; recorrido--)
    std::cout << binario[recorrido];
  std::cout << std::endl;
}


// Método para convetir a binario.
std::string
ConvertirBinario(u_int8_t numero) {

  std::string binario;

  // Dividimos el numero para pasarlo a binario
  while (numero != 0) {
    binario += (numero % 2 == 0 ? "0" : "1");
    numero /= 2;
  }
  // Rellenamos hasta que sean 8 valores.
  while (binario.size() != 8)
    binario += "0";

  return binario;
}


// Método para pasar una cantidad de numeros en hexadecimal que se encuentran en un string 
// a un valor numerico decimal.
u_int8_t
SacarNumeroHexa (std::string& aux_str) {
    
    u_int8_t pivote = 0;
    int contador = aux_str.size() - 1;

    // Recorremos el str y con potencias de 16 vamos pasando posicion a posicion a un valor
    // numérico mientras vamos sumando todos esos valores para dar con el número finalmente.
    for (unsigned desp_aux = 0; desp_aux < aux_str.size(); desp_aux++) {
      switch (aux_str[desp_aux])
      {
      case 'a':
        pivote += 10 * pow(16, contador);
        break;
      case 'b':
        pivote += 11  * pow(16, contador);
        break;
      case 'c':
        pivote += 12 * pow(16, contador);
        break;
      case 'd':
        pivote += 13 * pow(16, contador);
        break;
      case 'e':
        pivote += 14 * pow(16, contador);
        break;
      case 'f':
        pivote += 15 * pow(16, contador);
        break;
      
      default:
        pivote += (aux_str[desp_aux] - '0') * pow(16, contador);
        break;
      }
      contador--;
    }

    aux_str.clear(); 
    return pivote;
}

// Método para leer de teclado números sin importar el formato de entrada, devuelve un vector
// con los números.
void
TextoNumero (std::string cadena, std::vector<u_int8_t>& numeros) {

    std::string aux_str;

    // Recorremos el vector que contiene los números y otros caracteres.
    for (unsigned desp = 0; desp < cadena.size(); desp++) {
        
        // Si es un digito hacemos push_back en aux_str y cuando no sea un numero y sea ':'
        // y llevemos 4 tendremos nuestra primera palabra de 8 bits.
        if (isdigit(cadena[desp]) || cadena[desp] == 'a' || cadena[desp] == 'b' || cadena[desp] == 'c' 
                                  || cadena[desp] == 'd' || cadena[desp] == 'e' || cadena[desp] == 'f') 
          aux_str += cadena[desp];
        else if (aux_str.size() > 0) numeros.push_back(SacarNumeroHexa(aux_str));
    }
    // Cuando acabemos de revisar el str hay que mirar que en aux_str no quede nada, si queda algo
    // es que la cadena que entró por teclado acababa con un número que aún tenemos que pasar de texto
    // a decimal.
    if (aux_str.size() > 0) numeros.push_back(SacarNumeroHexa(aux_str));
}


// Método que desplaza para relizar la multiplicación.
u_int8_t
Desplazamiento(unsigned desp, u_int8_t valor, u_int8_t algoritmo) {

  // Auxiliar, si desplazamos el número hacia la izquierda y el número es
  // más pequeño que antes implica que hemos perdido un 1 y si el número
  // es más grande hemos perdido un 0.
  std::cout << "Iteración " << 0 << ": ";
  PasarBinario(valor);
  u_int8_t aux;
  for (unsigned contador = 0; contador < desp; contador++) {

    aux = valor;
    valor = valor << 1;
    std::cout << "Iteración " << contador + 1 << ": ";
    PasarBinario(valor);
    
    if (valor < aux) {

      valor ^= algoritmo;
      std::cout << "Aplicamos XOR: ";
      PasarBinario(valor);
    }
  }
  return valor;
}


int main(void) {

  std::string primerByte, segundoByte, algoritmo, segundoNumeroTexto;

  // Leemos de teclado
  std::cout << "Primer byte: ";
  getline(std::cin, primerByte);

  std::cout << "Segundo byte: ";
  getline(std::cin, segundoByte);

  std::cout << "Algoritmo: ";
  getline(std::cin, algoritmo);

  u_int8_t algorit = 0, resultadoFinal = 0;

  std::vector<u_int8_t> primerNumero, segundoNumero, resultado;

  // Pasamos los números en texto Hexadecimal a entero sin signo de 8 bits.
  TextoNumero(primerByte, primerNumero);
  TextoNumero(segundoByte, segundoNumero);
  segundoNumeroTexto = ConvertirBinario(segundoNumero[0]);

  // Miramos el algoritmo.
  if(algoritmo == "AES") {
    algorit = 27;
  } else {
    algorit = 169;
  }

  std::cout << "Byte Algoritmo: "; PasarBinario(algorit);
  std::cout << "Primer byte: "; PasarBinario(primerNumero[0]);
  std::cout << "Segundo byte: "; PasarBinario(segundoNumero[0]);
  std::cout << std:: endl;

  // Calculamos los diferentes valores, utilizando el segundo número como una
  // cadena de texto y de esta forma mirando los 1.
  for (unsigned aux = 0; aux < segundoNumeroTexto.size(); aux++) { 
    std::cout << "\nEl bit del segundo número a analizar es: " << segundoNumeroTexto[aux] << std::endl;
    std::cout << "Posición del bit a analizar :" << aux << std::endl;
    if (segundoNumeroTexto[aux] == '1') 
      resultado.push_back(Desplazamiento(aux, primerNumero[0], algorit));
  }

  // Aplicamos XOR a todos los resultados.
  for (unsigned aux = 0; aux < resultado.size(); aux++) {
    resultadoFinal ^= resultado[aux];
  }

  // Resultador final.
  std::cout << "\nMultiplicación: "; PasarBinario(resultadoFinal);
}