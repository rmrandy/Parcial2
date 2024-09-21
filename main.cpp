#include <iostream>
#include <pthread.h>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <vector>

using std::cout, std::cin, std::endl;
using sf::RenderWindow, sf::VideoMode, sf::RectangleShape, sf::Vector2f, sf::Event, sf::Color;

std::mutex seatMutex;

struct Seat {
    int seatNumber;
    Color color;
    bool isReserved;
    RectangleShape shape;
};

// Vector para almacenar los asientos
std::vector<Seat> seats;

// Función que será ejecutada por cada hilo para reservar asientos
void* reserveSeat(void* args) {
    int seatNumber = *(int*)args;

    // Simular tiempo de espera aleatorio (por ejemplo, para simular usuarios reservando en diferentes momentos)
    sf::sleep(sf::milliseconds(rand() % 5000));

    // Bloquear el acceso a la estructura de asientos
    seatMutex.lock();

    // Simular la reserva de un asiento
    seats[seatNumber - 1].isReserved = true;
    seats[seatNumber - 1].color = Color::Red;  // Reservar con color rojo

    cout << "Asiento " << seatNumber << " reservado.\n";

    // Desbloquear el acceso a la estructura de asientos
    seatMutex.unlock();

    return NULL;
}

// Función para dibujar los asientos en la ventana
void drawSeats(RenderWindow &window) {
    seatMutex.lock(); // Proteger el acceso a los asientos

    for (auto& seat : seats) {
        seat.shape.setFillColor(seat.color);
        window.draw(seat.shape);  // Dibujar el rectángulo del asiento
    }

    seatMutex.unlock(); // Desbloquear el acceso
}

// Función para manejar clics en los asientos
void handleSeatClick(RenderWindow &window) {
    seatMutex.lock(); // Proteger el acceso a los asientos

    for (auto& seat : seats) {
        // Verificar si el clic fue dentro del área del asiento
        if (seat.shape.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            if (seat.isReserved) {
                // Si el asiento está reservado, lo ponemos disponible (rojo)
                seat.color = Color::Green;
                seat.isReserved = false;
                cout << "Asiento " << seat.seatNumber << " ahora está disponible.\n";
            } else {
                // Si el asiento está disponible (rojo), lo reservamos de nuevo (verde)
                seat.color = Color::Green;
                seat.isReserved = true;
                cout << "Asiento " << seat.seatNumber << " ahora está reservado.\n";
            }
        }
    }

    seatMutex.unlock(); // Desbloquear el acceso
}

int main() {
    
    
    // Crear una ventana de 800x800 píxeles
    RenderWindow window(VideoMode(800, 800), "Reserva de Asientos");

    // Inicializar los asientos
    const int numSeats = 30;  // Número de asientos en total (cambia este valor para más asientos)
    const int seatsPerRow = 10;  // Asientos por fila
    const float seatSpacing = 30.f;  // Espacio entre asientos
    const float rowSpacing = 50.f;   // Espacio entre filas
    const float initialX = 30.f;  // Posición inicial en X
    const float initialY = 100.f; // Posición inicial en Y

    for (int i = 0; i < numSeats; i++) {
        Seat seat;
        seat.seatNumber = i + 1;
        seat.color = Color::White;  // Inicialmente todos los asientos están sin reservar
        seat.isReserved = false;    // No están reservados
        seat.shape.setSize(Vector2f(25.f, 25.f));

        // Calcular la posición del asiento basado en el número de asiento
        float x = initialX + seatSpacing * (i % seatsPerRow);  // Colocar asiento en X
        float y = initialY + rowSpacing * (i / seatsPerRow);   // Colocar asiento en Y para las filas

        seat.shape.setPosition(x, y);
        seats.push_back(seat);
    }

    // Crear múltiples hilos que simulan la reserva de asientos
    const int numUsers = numSeats;  // Número de usuarios simulados
    pthread_t seatThreads[numUsers];
    int seatNumbers[numUsers];

    for (int i = 0; i < numUsers; i++) {
        seatNumbers[i] = i + 1;  // Asignar un número de asiento para cada hilo
        pthread_create(&seatThreads[i], NULL, reserveSeat, &seatNumbers[i]);
    }

    // Bucle principal de la ventana
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                handleSeatClick(window);  // Manejar clics en los asientos
            }
        }

        // Limpiar la ventana
        window.clear(Color::Black);

        // Dibujar los asientos
        drawSeats(window);

        // Mostrar lo que se ha dibujado
        window.display();
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < numUsers; i++) {
        pthread_join(seatThreads[i], NULL);
    }

    return 0;
}
