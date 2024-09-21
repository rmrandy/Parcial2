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
};


std::vector<Seat> reservedSeats;

// Función que será ejecutada por cada hilo para reservar asientos
void* reserveSeat(void* args) {
    int seatNumber = *(int*)args;

    // Simular tiempo de espera aleatorio (por ejemplo, para simular usuarios reservando en diferentes momentos)
    sf::sleep(sf::milliseconds(rand() % 5000));

    // Bloquear el acceso a la estructura de asientos
    seatMutex.lock();

    // Simular la reserva de un asiento
    Seat seat;
    seat.seatNumber = seatNumber;
    seat.color = Color::Green;  // Reservar con color verde

    // Agregar el asiento reservado al vector
    reservedSeats.push_back(seat);

    cout << "Asiento " << seatNumber << " reservado.\n";

    // Desbloquear el acceso a la estructura de asientos
    seatMutex.unlock();

    return NULL;
}

// Función para dibujar los asientos reservados en la ventana
void drawSeats(RenderWindow &window) {
    seatMutex.lock(); // Proteger el acceso a los asientos reservados

    for (const auto& seat : reservedSeats) {
        RectangleShape rectangle(Vector2f(25.f, 25.f));
        rectangle.setFillColor(seat.color);
        rectangle.setPosition(30.f * seat.seatNumber, 100.f);  // Posición basada en el número de asiento
        window.draw(rectangle);
    }

    seatMutex.unlock(); // Desbloquear el acceso
}

int main() {
    // Crear una ventana de 800x800 píxeles
    RenderWindow window(VideoMode(800, 800), "Reserva de Asientos");

    // Crear múltiples hilos que simulan la reserva de asientos
    const int numUsers = 9;  // Número de usuarios simulados
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
        }

        // Limpiar la ventana
        window.clear(Color::Black);

        // Dibujar los asientos reservados
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