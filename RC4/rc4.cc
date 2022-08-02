#include <iostream>
#include <vector>

#include <math.h>



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
PasarBinario(uint8_t numero) {

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
}



// Método para pasar una cantidad de numeros que se encuentran en un string 
// a un valor numerico.
void
SacarNumero (std::string& aux_str, std::vector<uint8_t>& numeros) {
    
    uint8_t pivote;
    int aux_i;

    aux_i = aux_str.size() - 1;
    pivote = 0;
    // Recorremos el str y con potencias de 10 vamos pasando posicion a posicion a un valor
    // numérico mientras vamos sumando todos esos valores para dar con el número finalmente.
    for (unsigned desp_aux = 0; desp_aux < aux_str.size(); desp_aux++) {
        pivote += (aux_str[desp_aux] - '0') * pow(10, aux_i);
        aux_i--;
    }
    numeros.push_back(pivote);
    aux_str.clear(); 
}



// Método para leer de teclado números sin importar el formato de entrada, devuelve un vector
// con los números.
std::vector<uint8_t>
TextoNumero (std::string cadena) {

    std::vector<uint8_t> numeros;
    std::string aux_str;

    // Recorremos el vector que contiene los números y otros caracteres.
    for (unsigned desp = 0; desp < cadena.size(); desp++) {
        
        // Si es un digito hacemos push_back en aux_str y cuando no sea un numero es porque ha 
        // encontrado algo que separa a los números y procedemos a sacar el número de aux_str.
        if (isdigit(cadena[desp])) aux_str += cadena[desp];
        else if (aux_str.size() > 0) SacarNumero(aux_str, numeros);
    }
    // Cuando acabemos de revisar el str hay que mirar que en aux_str no quede nada, si queda algo
    // es que la cadena que entró por teclado acababa con un número que aún tenemos que pasar de texto
    // a decimal.
    if (aux_str.size() > 0) SacarNumero(aux_str, numeros);

    return numeros;
}



// Método que inicializa S con valores del 0 al 255. Imprime S con un cierto 
// formato.
std::vector<uint8_t>
InicializarS () {

    std::vector<uint8_t> auxS;
    unsigned contador = 1;
    auxS.resize(256);

    std::cout << "S=[";
    for (unsigned desp = 0; desp <= 255; desp++) {
        auxS[desp] = desp;
        
        if (contador % 16 == 0 && desp != 255) std::cout << desp << "\n   ";
        else if (desp != 255)                  std::cout << desp << ", ";

        contador ++;
    }
    std::cout << int(auxS[auxS.size() -1]) << "]\n\n";
    return auxS;
}



// Método que inicializa K rellenandolo con los valores introducidos por teclado como 
// semilla de clave, lo hace hasta rellenar 256 posiciones (0-255). Imprime K con un
// cierto formato.
std::vector<uint8_t>
InicializarK (std::vector<uint8_t>& numeros) {

    std::vector<uint8_t> auxK;
    unsigned contador = 1;
    
    auxK.resize(256);

    std::cout << "K=[";
    for (unsigned desp = 0; desp <= 255; desp++) {
        auxK[desp] = numeros[desp % numeros.size()];

        if (contador % 16 == 0 && desp != 255)  std::cout << int(auxK[desp]) << "\n   ";
        else if (desp != 255)                   std::cout << int(auxK[desp]) << ", ";

        contador ++;
    }
    std::cout << int(auxK[auxK.size() -1]) << "]\n\n";
    return auxK;
}



// Método que imprime S con un cierto formato.
void
ImprimirS (std::vector<uint8_t>& auxS) {

    unsigned contador = 1;

    std::cout << "S=[";
    for (unsigned desp = 0; desp < auxS.size(); desp++) {

        if (contador % 16 == 0 && desp != 255) std::cout << int(auxS[desp]) << "\n   ";
        else if (desp != 255)           std::cout << int(auxS[desp]) << ", ";

        contador ++;
    }
    std::cout << int(auxS[auxS.size() -1]) << "]\n\n";
}



// Algoritmo KSA de RC4, que inicializa S y K, además de desordenar S.
void
KSA (std::vector<uint8_t> numeros, std::vector<uint8_t>& k, std::vector<uint8_t>& s) {

    s = InicializarS();
    k = InicializarK(numeros);

    unsigned j = 0;
    uint8_t pivote;
    for (unsigned i = 0; i <= 255; i++) {

        j = (j + k[i] + s[i]) % 256;

        std::cout << "S[" << i << "]=" << int(s[i]) << " K[" << i << "]=" << int(k[i]) << " produce " << "f=" << j << " y ";
        pivote = s[i];
        s[i] = s[j];
        s[j] = pivote;
        std::cout << "S[" << i << "]=" << int(s[i]) << " S[" << j << "]=" << int(s[j]) << std::endl;
    }
    std::cout << std::endl;
    ImprimirS(s);
}



// Método PRGA de RC$, que nos genera las posiciones t en S que serán los valores con los
// cuales cifrar el mensaje original.
void
PRGA (std::vector<uint8_t>& s, std::vector<uint8_t>& keyStream, unsigned& i, unsigned& j) {

    unsigned t;
    uint8_t pivote;

        i = (i +1) % 256;
        j = (j + s[i]) % 256;

        pivote = s[i];
        s[i] = s[j];
        s[j] = pivote;

        t = (s[i] + s[j]) % 256;
        std::cout << t << "]=" << int(s[t]) << "     ";
        keyStream.push_back(s[t]);
}



int main(void) {

    // Inicializamos las variables.
    std::vector<uint8_t> mensaje, keyStream, mensaje_cifrado, k, s;
    std::string semilla, texto;
    unsigned i = 0, j = 0, cont = 1;
    
    // Pedimos la Semilla de clave y el texto original por teclado.
    std::cout << "Semilla de clave: ";
    getline(std::cin, semilla);
    std::cout << "Texto original: ";
    getline(std::cin, texto);

    // Desordenamos S, usando KSA
    std::cout << "~~~~~INICIALIZACION~~~~~\n";
    KSA(TextoNumero(semilla), k, s);

    // Pasamos el mensaje introducido por teclado a decimal.
    mensaje = TextoNumero(texto);

    std::cout << "~~~~~GENERACION DE SECUENCIA CIFRANTE Y TEXTO CIFRADO~~~~~\n";
    // Ejecutamos el algoritmo PRGA tantas veces como letras del mensaje introducido.
    while (cont <= mensaje.size()) {
    
        // Calculamos la posición de S (t) que será el valor S[t] al cual se le aplicará la operación 
        // XOR con el mensaje original.
        std::cout << "Byte " << cont << " de secuencia cifrante: Salida = S[";
        PRGA(s, keyStream, i, j);
        // Pasamos el valor S[t] a binario.
        PasarBinario(keyStream[cont - 1]);
        std::cout << std::endl;
    
        // Valor decimal del Mensaje[cont - 1].
        std::cout << "Byte " << cont << " de texto original: Entrada: M[" << cont << "]=" << int(mensaje[cont - 1]) << "     ";
        // Pasamos a binario el valor decimal del mensaje
        PasarBinario(mensaje[cont - 1]);
        std::cout << std::endl;

        // Realizamos la operación XOR entre el Mensaje[cont - 1] y el S[t]
        mensaje_cifrado.push_back(mensaje[cont - 1] ^ keyStream[cont - 1]);
        std::cout << "Byte " << cont << " de texto cifrado, Salida = C[" << cont << "]=" << int(mensaje_cifrado[cont - 1]) << "     ";
        // Pasamos a binario el mensaje cifrado.
        PasarBinario(mensaje_cifrado[cont - 1]);
        std::cout << std::endl;

        cont++;
    }
}