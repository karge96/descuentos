#include <iostream>
#include <iomanip> 
#include <vector> 
#include <limits>
//definiendo la estructura de un producto 
struct Producto {
    std::string nombre;
    double precio;
    std::string sku;
};
//estructura de la venta realizada
struct Venta {
    Producto producto;
    int cantidad;
    double subtotal;
    double descuento;
    double total;
};
//lista de los productos disponibles para la venta
std::vector<Producto> listaProductos = {
    {"Manzanas", 300, "0SKU001"},
    {"Platanos", 450, "0SKU002"},
    {"Tomates", 400, "0SKU003"},
    {"Lechuga", 550, "0SKU004"},
    {"Avena", 200, "0SKU005"},
    {"Detergente", 5000, "2SKU006"},
    {"Jabon", 2000, "2SKU007"},
    {"Shampoo", 9000, "2SKU008"}
};
//Guarda el nombre del cliente
void clienteNombre(std::string &nombreCliente) {
    std::cout << "Ingrese el nombre del cliente: ";
    std::getline(std::cin, nombreCliente);
}
//buscamos el producto ingresado en la lista disponible
Producto buscarProducto(const std::string &nombreProducto) {
    for (const auto &producto : listaProductos) {
        if (producto.nombre == nombreProducto) {
            return producto;
        }
    }
    // Si el producto no se encuentra, retornamos un producto vacío con precio -1
    return {"", -1, ""};
}

void realizarventa(std::vector<Venta> &ventas) {
    char continuar;
    do {
        Venta venta;
        Producto productoEncontrado;

        do { //pediremos el nombre del producto
            std::cout << "Ingrese el nombre del producto: ";
            std::getline(std::cin, venta.producto.nombre);

            productoEncontrado = buscarProducto(venta.producto.nombre);
            if (productoEncontrado.precio == -1) { //si el producto no se encuentra
                std::cout << "Producto no encontrado. Por favor, intente nuevamente.\n";
            }
        } while (productoEncontrado.precio == -1);

        venta.producto = productoEncontrado;
        //ingresar numeros validos para la venta
        std::cout << "Ingrese la cantidad del producto: ";
        while (!(std::cin >> venta.cantidad) || venta.cantidad <= 0) {
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "Cantidad no valida. Por favor, ingrese un numero valido: ";
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        //calculamos el subtotal
        venta.subtotal = productoEncontrado.precio * venta.cantidad;
              
        // Inicialmente sin descuento
        venta.descuento = 0;
        venta.total = venta.subtotal;

        ventas.push_back(venta);

        std::cout << "Desea agregar otro producto? (s/n): ";
        std::cin >> continuar;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    } while (continuar == 's' || continuar == 'S');
}

// Aplicar descuento solo a los productos que califican y cuya compra supera los mil pesos
void aplicarDescuentos(std::vector<Venta> &ventas) {
    for (auto &venta : ventas) {
        if (venta.producto.sku[0] == '0' && venta.subtotal >= 1000) {

            venta.descuento = venta.subtotal * 0.20;
            venta.total = venta.subtotal - venta.descuento;
        }
    }
}

//calcula el total de la compra
void calcularTotales(const std::vector<Venta> &ventas, double &subtotal, double &descuento, double &total) {
    subtotal = 0.0;
    descuento = 0.0;
    total = 0.0;
    //por cada venta buscamos el total
    for (const auto &venta : ventas) {
        subtotal += venta.subtotal;
        descuento += venta.descuento;
        total += venta.total;
    }
}
//mostramos todos los datos de la compra en formato ticket de venta
void factura(const std::string &nombreCliente, const std::vector<Venta> &ventas, double subtotal, double descuento, double total) {
    int columna = 15;  // Ancho para las columnas de texto
    int columnaPrecio = 10;   // Ancho para las columnas de precios y cantidades

    std::cout << "\n--- Factura ---\n";
    std::cout << "Nombre del cliente: " << nombreCliente << "\n\n";

    // Encabezado del ticket
    std::cout << std::left << std::setw(columna) << "SKU"
            << std::left << std::setw(columna) << "Detalle"
            << std::right << std::setw(columnaPrecio) << "Cantidad"
            << std::right << std::setw(columnaPrecio) << "Precio"
            << std::right << std::setw(columnaPrecio) << "Subtotal"
            << std::right << std::setw(columnaPrecio) << "Descuento"
            << std::right << std::setw(columnaPrecio) << "Total"
            << "\n";

    // Muestra el detalle de cada producto vendido
    for (const auto &venta : ventas) {
        std::cout << std::left << std::setw(columna) << venta.producto.sku
                << std::left << std::setw(columna) << venta.producto.nombre
                << std::right << std::setw(columnaPrecio) << venta.cantidad
                << std::right << std::setw(columnaPrecio) << std::fixed << std::setprecision(2) << venta.producto.precio
                << std::right << std::setw(columnaPrecio) << venta.subtotal
                << std::right << std::setw(columnaPrecio) << venta.descuento
                << std::right << std::setw(columnaPrecio) << venta.total
                << "\n";
    }

    // Se muestra el total de toda la compra
    std::cout << std::left << std::setw(columna * 2) << "Subtotal:" 
            << std::right << std::setw(columna * 4) << subtotal << "\n";
    std::cout << std::left << std::setw(columna * 2) << "Descuento Total:" 
            << std::right << std::setw(columna * 4) << descuento << "\n";
    std::cout << std::left << std::setw(columna * 2) << "Total:" 
            << std::right << std::setw(columna * 4) << total << "\n";
}

int main() {
    std::string nombreCliente;
    std::vector<Venta> ventas;

    clienteNombre(nombreCliente);
    realizarventa(ventas);
    aplicarDescuentos(ventas);
    double subtotal, descuento, total;
    calcularTotales(ventas, subtotal, descuento, total);
    factura(nombreCliente, ventas, subtotal, descuento, total);

    return 0;
}
