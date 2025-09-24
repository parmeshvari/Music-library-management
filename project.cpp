#include <iostream>    // For I/O (cin, cout)
#include <string>      // For std::string
#include <fstream>     // For file handling
#include <limits>      // For numeric_limits
#include <stdexcept>   // For exceptions

using namespace std;
// 2. STRUCTURES
struct Artist {
    string name;
    string genre;
    int yearBorn;

    // Marked const so it can be called inside const functions
    void display() const {
        cout << "Artist: " << name << " (Genre: " << genre << ", Born: " << yearBorn << ")" << endl;
    }
};

// =========================================
// 8. BASE CLASS (INHERITANCE)
class Media {
protected:
    string title;

public:
    Media(string t = "Unknown") : title(t) {}
    virtual void play() {
        cout << "Playing media: " << title << endl;
    }
    virtual ~Media() {}
    string getTitle() const { return title; }
};

// =========================================
// SONG CLASS
class Song : public Media {
private:
    Artist artist;
    double duration;
    static int totalSongs;

public:
    Song() : Media("Untitled"), duration(0.0) {
        totalSongs++;
        cout << "Default Song constructor called" << endl;
    }

    Song(string t, Artist a, double d) : Media(t), artist(a), duration(d) {
        if (d < 0) throw invalid_argument("Duration can't be negative!");
        totalSongs++;
        cout << "Parameterized Song constructor called for " << title << endl;
    }

    Song(const Song& other) : Media(other.title), artist(other.artist), duration(other.duration) {
        totalSongs++;
        cout << "Copy Song constructor called" << endl;
    }

    ~Song() {
        cout << "Song destructor called for " << title << endl;
        totalSongs--;
    }

    void display() const {
        cout << "Song: " << title << " by ";
        artist.display();
        cout << "Duration: " << duration << " min (const function)" << endl;
    }

    void display(bool showArtistDetails) const {
        display();
        if (showArtistDetails) {
            cout << "Extra: Artist details shown!" << endl;
        }
    }

    void scaleDuration(double factor) {
        this->duration *= factor;
        cout << "Duration scaled using 'this': " << this->duration << endl;
    }

    friend void printPrivateSongInfo(const Song& s);
    void play() override {
        cout << "Playing song: " << title << " (overridden)" << endl;
    }

    Song operator+(const Song& other) const {
        return Song(title + " + " + other.title, artist, duration + other.duration);
    }

    friend ostream& operator<<(ostream& os, const Song& s);

    double getDuration() const { return duration; }
    string getTitle() const { return title; }
    static int getTotalSongs() { return totalSongs; }
};

int Song::totalSongs = 0;

void printPrivateSongInfo(const Song& s) {
    cout << "Friend accessing private: Duration = " << s.duration << endl;
}

ostream& operator<<(ostream& os, const Song& s) {
    os << s.title << " (" << s.duration << " min)";
    return os;
}

// =========================================
// ALBUM CLASS
class Album : public Song {
private:
    Song* songs;
    int numSongs;
    const int maxSongs = 10;

public:
    Album(string albumTitle, Artist a) : Song(albumTitle, a, 0.0), numSongs(0) {
        songs = new Song[maxSongs];
        cout << "Album constructor called" << endl;
    }

    ~Album() {
        delete[] songs;
        cout << "Album destructor called" << endl;
    }

    void addSong(const Song& newSong) {
        try {
            if (numSongs >= maxSongs) throw overflow_error("Album full!");
            songs[numSongs++] = newSong;
            cout << "Song added to album" << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void play() override {
        cout << "Playing album: " << getTitle() << " (album override)" << endl;
        for (int i = 0; i < numSongs; ++i) {
            songs[i].play();
        }
    }

    void displayAlbum() const {
        cout << "Album: " << getTitle() << " with " << numSongs << " songs:" << endl;
        for (int i = 0; i < numSongs; ++i) {
            cout << "  " << songs[i] << endl;
        }
    }

    int getNumSongs() const { return numSongs; }
    Song* getSongs() const { return songs; }
};

// =========================================
// MULTIPLE INHERITANCE DEMO
class Exportable {
public:
    virtual void exportData() { cout << "Exporting data" << endl; }
};

class PlayableItem : public Media, public Exportable {
public:
    PlayableItem(string t = "Unnamed") : Media(t) {}  // Added constructor
    void play() override { cout << "Playable item playing (multiple inheritance)" << endl; }
    void exportData() override { cout << "Exporting playable item" << endl; }
};

// =========================================
// TEMPLATE FUNCTIONS/CLASSES
template <typename T>
void swapItems(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
    cout << "Swapped using function template" << endl;
}

template <typename T>
class Container {
private:
    T* items;
    int size;

public:
    Container(int s) : size(s) {
        items = new T[size];
    }

    ~Container() { delete[] items; }

    void add(int index, const T& item) {
        if (index < size) items[index] = item;
    }

    T search(double value) const {
        for (int i = 0; i < size; ++i) {
            if (items[i].getDuration() > value) {
                return items[i];
            }
        }
        return T();
    }
};

// =========================================
// MUSIC LIBRARY
class MusicLibrary {
private:
    Album** albums;
    int numAlbums;
    const int maxAlbums = 5;
    static int libraryID;

public:
    MusicLibrary() : numAlbums(0) {
        albums = new Album*[maxAlbums];
        libraryID++;
        cout << "MusicLibrary constructor called (ID: " << libraryID << ")" << endl;
    }

    ~MusicLibrary() {
        for (int i = 0; i < numAlbums; ++i) {
            delete albums[i];
        }
        delete[] albums;
        cout << "MusicLibrary destructor called" << endl;
    }

    void addAlbum(string title, Artist a) {
        try {
            if (numAlbums >= maxAlbums) throw overflow_error("Library full!");
            albums[numAlbums++] = new Album(title, a);
            cout << "Album added" << endl;
        } catch (const exception& e) {
            cout << "Error adding album: " << e.what() << endl;
        }
    }

    void addSongToAlbum(int albumIndex, const Song& song) {
        if (albumIndex >= 0 && albumIndex < numAlbums) {
            albums[albumIndex]->addSong(song);
        } else {
            throw out_of_range("Invalid album index!");
        }
    }

    void displayLibrary() const {
        cout << "\n=== Music Library (Total Songs Created: " << Song::getTotalSongs() << ") ===" << endl;
        for (int i = 0; i < numAlbums; ++i) {
            albums[i]->displayAlbum();
            albums[i]->play();
            if (albums[i]->getNumSongs() > 0) {
                printPrivateSongInfo(albums[i]->getSongs()[0]);
            }
        }
        cout << "Total Albums: " << numAlbums << endl;
    }

    void searchSongs(double minDuration) {
        if (numAlbums == 0) {
            cout << "No albums to search." << endl;
            return;
        }
        Container<Song> searcher(10);
        for (int i = 0; i < albums[0]->getNumSongs() && i < 10; ++i) {
            searcher.add(i, albums[0]->getSongs()[i]);
        }

        Song found = searcher.search(minDuration);
        if (found.getDuration() > 0) {
            cout << "Found song longer than " << minDuration << ": " << found << endl;
        } else {
            cout << "No song found longer than " << minDuration << endl;
        }
    }

    void combineAlbums(int idx1, int idx2) {
        if (idx1 >= 0 && idx1 < numAlbums && idx2 >= 0 && idx2 < numAlbums &&
            albums[idx1]->getNumSongs() > 0 && albums[idx2]->getNumSongs() > 0) {
            Song combined = albums[idx1]->getSongs()[0] + albums[idx2]->getSongs()[0];
            cout << "Combined song: " << combined << endl;
        } else {
            cout << "Cannot combine: Invalid indices or empty albums." << endl;
        }
    }

    void demoMultipleInheritance() {
        PlayableItem item("Demo Item");
        item.play();
        item.exportData();
    }

    Song& getFirstSong() {
        if (numAlbums > 0 && albums[0]->getNumSongs() > 0) {
            return albums[0]->getSongs()[0];
        }
        throw out_of_range("No songs available for scaling.");
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file) {
            throw runtime_error("Can't open file for writing!");
        }
        file << "Music Library - Total Songs: " << Song::getTotalSongs() << endl;
        for (int i = 0; i < numAlbums; ++i) {
            file << "Album: " << albums[i]->getTitle() << endl;
            for (int j = 0; j < albums[i]->getNumSongs(); ++j) {
                file << "  Song: " << albums[i]->getSongs()[j].getTitle()
                     << " (" << albums[i]->getSongs()[j].getDuration() << " min)" << endl;
            }
        }
        file.close();
        cout << "Library saved to " << filename << endl;
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "No file found, starting empty library." << endl;
            return;
        }
        string line;
        cout << "Loading library..." << endl;
        while (getline(file, line)) {
            cout << "Loaded line: " << line << endl;
        }
        file.close();
    }

    static int getLibraryID() { return libraryID; }
    int getNumAlbums() const { return numAlbums; }  // FIX for private access
};

int MusicLibrary::libraryID = 0;

// =========================================
// MAIN
int main() {
    MusicLibrary library;
    int choice;
    Artist defaultArtist = {"Unknown", "Unknown", 2000};

    try {
        library.loadFromFile("library.txt");
    } catch (const exception& e) {
        cout << "Load error: " << e.what() << endl;
    }

    do {
        cout << "\n=== Music Library Menu ===" << endl;
        cout << "1. Add Album" << endl;
        cout << "2. Add Song to Album (index 0 for demo)" << endl;
        cout << "3. Display Library" << endl;
        cout << "4. Search Songs (> duration)" << endl;
        cout << "5. Scale Song Duration (demo on first song)" << endl;
        cout << "6. Combine Albums (0 and 1)" << endl;
        cout << "7. Demo Multiple Inheritance" << endl;
        cout << "8. Swap Two Songs (template demo)" << endl;
        cout << "9. Save Library" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        try {
            switch (choice) {
                case 1: {
                    string title;
                    string artistName, genre;
                    int year;
                    cout << "Enter album title: ";
                    getline(cin, title);
                    cout << "Enter artist name: ";
                    getline(cin, artistName);
                    cout << "Enter genre: ";
                    getline(cin, genre);
                    cout << "Enter year born: ";
                    cin >> year;
                    cin.ignore();
                    Artist a = {artistName, genre, year};
                    library.addAlbum(title, a);
                    break;
                }
                case 2: {
                    if (library.getNumAlbums() == 0) {
                        cout << "Add an album first!" << endl;
                        break;
                    }
                    string songTitle;
                    string artistName, genre;
                    int year;
                    double dur;
                    cout << "Enter song title: ";
                    getline(cin, songTitle);
                    cout << "Enter artist name: ";
                    getline(cin, artistName);
                    cout << "Enter genre: ";
                    getline(cin, genre);
                    cout << "Enter year born: ";
                    cin >> year;
                    cout << "Enter duration (min): ";
                    cin >> dur;
                    cin.ignore();
                    Artist a = {artistName, genre, year};
                    Song newSong(songTitle, a, dur);
                    library.addSongToAlbum(0, newSong);
                    break;
                }
                case 3:
                    library.displayLibrary();
                    break;
                case 4: {
                    double minDur;
                    cout << "Enter minimum duration: ";
                    cin >> minDur;
                    cin.ignore();
                    library.searchSongs(minDur);
                    break;
                }
                case 5: {
                    try {
                        double factor;
                        cout << "Enter scaling factor: ";
                        cin >> factor;
                        cin.ignore();
                        Song& s = library.getFirstSong();
                        s.scaleDuration(factor);
                    } catch (const exception& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                    break;
                }
                case 6:
                    library.combineAlbums(0, 1);
                    break;
                case 7:
                    library.demoMultipleInheritance();
                    break;
                case 8: {
                    try {
                        if (library.getNumAlbums() > 0 && library.getFirstSong().getDuration() > 0) {
                            Song s1 = library.getFirstSong();
                            Song s2 = s1;
                            cout << "Before swap: " << s1 << " and " << s2 << endl;
                            swapItems(s1, s2);
                            cout << "After swap: " << s1 << " and " << s2 << endl;
                        } else {
                            cout << "Need at least one song to demo swap!" << endl;
                        }
                    } catch (const exception& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                    break;
                }
                case 9:
                    try {
                        library.saveToFile("library.txt");
                    } catch (const exception& e) {
                        cout << "Save error: " << e.what() << endl;
                    }
                    break;
                case 0:
                    cout << "Exiting program..." << endl;
                    break;
                default:
                    cout << "Invalid choice. Try again." << endl;
                    break;
            }
        } catch (const exception& e) {
            cout << "Menu error: " << e.what() << endl;
        }
    } while (choice != 0);

    return 0;
}
