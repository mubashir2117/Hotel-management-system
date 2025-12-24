#include <iostream>
#include <string>
#include <sstream>
#include <queue>
using namespace std;

// Room Node for BST
struct Room {
    int roomNo;
    string roomType;  // Single, Double, Suite
    int price;
    bool isBooked;
    string guestName;
    string checkInDate;
    Room* left;
    Room* right;
    
    Room(int no, string type, int p) {
        roomNo = no;
        roomType = type;
        price = p;
        isBooked = false;
        guestName = "";
        checkInDate = "";
        left = right = NULL;
    }
};

// Booking Node for History (Linked List)
struct Booking {
    int bookingId;
    int roomNo;
    string guestName;
    string checkInDate;
    Booking* next;
};

// Waiting List Node (Queue)
struct WaitingGuest {
    string name;
    string roomType;
    string date;
};

class HotelManagement {
private:
    Room* root;
    Booking* bookingHead;
    queue<WaitingGuest> waitingList;
    int bookingCounter;
    
    // Insert room in BST
    Room* insertRoom(Room* node, int roomNo, string type, int price) {
        if (node == NULL) {
            return new Room(roomNo, type, price);
        }
        
        if (roomNo < node->roomNo) {
            node->left = insertRoom(node->left, roomNo, type, price);
        } else if (roomNo > node->roomNo) {
            node->right = insertRoom(node->right, roomNo, type, price);
        }
        
        return node;
    }
    
    // Search room in BST
    Room* searchRoom(Room* node, int roomNo) {
        if (node == NULL || node->roomNo == roomNo) {
            return node;
        }
        
        if (roomNo < node->roomNo) {
            return searchRoom(node->left, roomNo);
        }
        
        return searchRoom(node->right, roomNo);
    }
    
    // In-order traversal for display
    void inorderDisplay(Room* node, string& result) {
        if (node != NULL) {
            inorderDisplay(node->left, result);
            
            result += to_string(node->roomNo) + "," + node->roomType + "," + 
                     to_string(node->price) + "," + 
                     (node->isBooked ? "Booked" : "Available") + "," +
                     node->guestName + ";";
            
            inorderDisplay(node->right, result);
        }
    }
    
    // Display available rooms by type
    void displayByType(Room* node, string type, string& result) {
        if (node != NULL) {
            displayByType(node->left, type, result);
            
            if (node->roomType == type && !node->isBooked) {
                result += to_string(node->roomNo) + "," + node->roomType + "," + 
                         to_string(node->price) + ";";
            }
            
            displayByType(node->right, type, result);
        }
    }
    
public:
    HotelManagement() {
        root = NULL;
        bookingHead = NULL;
        bookingCounter = 1;
        
        // Initialize hotel with rooms
        initializeRooms();
    }
    
    void initializeRooms() {
        // Single Rooms (101-105)
        root = insertRoom(root, 101, "Single", 1000);
        root = insertRoom(root, 102, "Single", 1000);
        root = insertRoom(root, 103, "Single", 1000);
        root = insertRoom(root, 104, "Single", 1000);
        root = insertRoom(root, 105, "Single", 1000);
        
        // Double Rooms (201-205)
        root = insertRoom(root, 201, "Double", 2000);
        root = insertRoom(root, 202, "Double", 2000);
        root = insertRoom(root, 203, "Double", 2000);
        root = insertRoom(root, 204, "Double", 2000);
        root = insertRoom(root, 205, "Double", 2000);
        
        // Suite Rooms (301-303)
        root = insertRoom(root, 301, "Suite", 5000);
        root = insertRoom(root, 302, "Suite", 5000);
        root = insertRoom(root, 303, "Suite", 5000);
    }
    
    void bookRoom(int roomNo, string guestName, string checkInDate) {
        Room* room = searchRoom(root, roomNo);
        
        if (room == NULL) {
            cout << "ERROR|Room " << roomNo << " does not exist" << endl;
            return;
        }
        
        if (room->isBooked) {
            cout << "ERROR|Room " << roomNo << " is already booked" << endl;
            return;
        }
        
        room->isBooked = true;
        room->guestName = guestName;
        room->checkInDate = checkInDate;
        
        // Add to booking history
        Booking* newBooking = new Booking();
        newBooking->bookingId = bookingCounter++;
        newBooking->roomNo = roomNo;
        newBooking->guestName = guestName;
        newBooking->checkInDate = checkInDate;
        newBooking->next = bookingHead;
        bookingHead = newBooking;
        
        cout << "SUCCESS|Room " << roomNo << " booked successfully for " << guestName 
             << "|Booking ID: " << newBooking->bookingId << endl;
    }
    
    void checkoutRoom(int roomNo) {
        Room* room = searchRoom(root, roomNo);
        
        if (room == NULL) {
            cout << "ERROR|Room " << roomNo << " does not exist" << endl;
            return;
        }
        
        if (!room->isBooked) {
            cout << "ERROR|Room " << roomNo << " is not booked" << endl;
            return;
        }
        
        string guestName = room->guestName;
        room->isBooked = false;
        room->guestName = "";
        room->checkInDate = "";
        
        cout << "SUCCESS|Room " << roomNo << " checked out successfully|Guest: " 
             << guestName << endl;
    }
    
    void displayAllRooms() {
        if (root == NULL) {
            cout << "EMPTY|No rooms available" << endl;
            return;
        }
        
        string result = "";
        inorderDisplay(root, result);
        
        if (result.empty()) {
            cout << "EMPTY|" << endl;
        } else {
            cout << "ROOMS|" << result << endl;
        }
    }
    
    void displayAvailableRooms(string type) {
        string result = "";
        displayByType(root, type, result);
        
        if (result.empty()) {
            cout << "EMPTY|No " << type << " rooms available" << endl;
        } else {
            cout << "AVAILABLE|" << result << endl;
        }
    }
    
    void searchRoomInfo(int roomNo) {
        Room* room = searchRoom(root, roomNo);
        
        if (room == NULL) {
            cout << "ERROR|Room not found" << endl;
            return;
        }
        
        cout << "FOUND|" << room->roomNo << "," << room->roomType << "," 
             << room->price << "," << (room->isBooked ? "Booked" : "Available") 
             << "," << room->guestName << "," << room->checkInDate << endl;
    }
    
    void addToWaitingList(string name, string roomType, string date) {
        WaitingGuest guest;
        guest.name = name;
        guest.roomType = roomType;
        guest.date = date;
        waitingList.push(guest);
        
        cout << "SUCCESS|Added " << name << " to waiting list" << endl;
    }
    
    void displayWaitingList() {
        if (waitingList.empty()) {
            cout << "EMPTY|No guests in waiting list" << endl;
            return;
        }
        
        queue<WaitingGuest> temp = waitingList;
        string result = "";
        
        while (!temp.empty()) {
            WaitingGuest guest = temp.front();
            result += guest.name + "," + guest.roomType + "," + guest.date + ";";
            temp.pop();
        }
        
        cout << "WAITING|" << result << endl;
    }
    
    void displayBookingHistory() {
        if (bookingHead == NULL) {
            cout << "EMPTY|No booking history" << endl;
            return;
        }
        
        Booking* temp = bookingHead;
        string result = "";
        
        while (temp != NULL) {
            result += to_string(temp->bookingId) + "," + to_string(temp->roomNo) 
                     + "," + temp->guestName + "," + temp->checkInDate + ";";
            temp = temp->next;
        }
        
        cout << "HISTORY|" << result << endl;
    }
};

int main() {
    HotelManagement hotel;
    string command;
    
    while (getline(cin, command)) {
        stringstream ss(command);
        string action;
        ss >> action;
        
        if (action == "BOOK") {
            int roomNo;
            string guestName, checkInDate;
            ss >> roomNo >> guestName >> checkInDate;
            hotel.bookRoom(roomNo, guestName, checkInDate);
        }
        else if (action == "CHECKOUT") {
            int roomNo;
            ss >> roomNo;
            hotel.checkoutRoom(roomNo);
        }
        else if (action == "DISPLAYALL") {
            hotel.displayAllRooms();
        }
        else if (action == "AVAILABLE") {
            string type;
            ss >> type;
            hotel.displayAvailableRooms(type);
        }
        else if (action == "SEARCH") {
            int roomNo;
            ss >> roomNo;
            hotel.searchRoomInfo(roomNo);
        }
        else if (action == "WAITLIST") {
            string name, roomType, date;
            ss >> name >> roomType >> date;
            hotel.addToWaitingList(name, roomType, date);
        }
        else if (action == "SHOWWAIT") {
            hotel.displayWaitingList();
        }
        else if (action == "HISTORY") {
            hotel.displayBookingHistory();
        }
        else if (action == "EXIT") {
            break;
        }
    }
    
    return 0;
}