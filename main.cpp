#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <regex>
#include <sstream>
#include <filesystem>


#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>

#include <sqlite_modern_cpp.h>
#include <sqlite_modern_cpp/errors.h>


int main()
{
    std::filesystem::path path("Tickets");
    std::filesystem::directory_iterator fsEnd;
    try
    {
        std::string dbName = "SQLiteContainer/sqlite_volume/TestDataBase.db"; 
        sqlite::database db(dbName);
        for(auto fsIterator = std::filesystem::directory_iterator(path); fsIterator != fsEnd; ++fsIterator)
        {
            std::cout << fsIterator->path() << "\n";
            std::string pathToFile = fsIterator->path();
            std::unique_ptr<poppler::document> doc(poppler::document::load_from_file(pathToFile));
            if(!doc)
            {
                std::cout << "Error with a document\n";
                return 1;
            }

            for(int i = 0; i < doc->pages(); ++i)
            {
                poppler::page* pageI = doc->create_page(i);
                std::string text = (pageI->text()).to_latin1();
                std::cout << text << "\n";
                std::regex record_regex(R"(Title:\s*(.+)\nDate:\s*(\d{1,4}/\d{1,2}/\d{1,4})\nTime:\s*(\d{1,2}:\d{2})(\s*[APap][Mm])?)");
                // Use sregex_iterator to find all matches
                auto begin = std::sregex_iterator(text.begin(), text.end(), record_regex);
                auto end = std::sregex_iterator();
                for(; begin != end; ++begin)
                {
                    std::smatch match= *begin;
                    std::string Title = match[1].str();
                    std::string Date =  match[2].str();;
                    std::string Time =  match[3].str();
                    std::string AMPMExtension = match[4].str();
                    std::cout << Title << " | " << Date << " | " << Time << " | " << AMPMExtension << "\n";
                    if(!AMPMExtension.empty())
                    {
                        int hour = std::stoi(Time.substr(0, Time.find(':')));
                        std::string minutes = Time.substr(Time.find(':') + 1);
                        if (AMPMExtension == "AM" || AMPMExtension == "am") {
                            if (hour == 12) {
                                hour = 0; // Convert 12 AM to 00
                            }
                        } else if (AMPMExtension == "PM" || AMPMExtension == "pm") {
                            if (hour != 12) {
                                hour += 12; // Add 12 for PM times other than 12 PM
                            }
                        }

                        Time = (hour < 10 ? "0" : "") + std::to_string(hour) + ":" + minutes;
                    }
                    
                    // yyyy-mm-dd
                    std::size_t firstSlash = Date.find_first_of("/");
                    std::size_t lastSlash = Date.find_last_of("/");
                    
                    // good, if i get in yyyy-mm-dd
                    std::string year = Date.substr(0, firstSlash);
                    std::string month = Date.substr(firstSlash + 1, lastSlash - firstSlash - 1);
                    if(month.size() == 1)
                    {
                        month.insert(month.begin(), '0');
                    }
                    std::string day = Date.substr(lastSlash + 1);
                    // else we have to rearange if we get the dd-mm-yyyy
                    if(day.size() == 4)
                    {
                        std::swap(year, day);
                    }
                    if(day.size() == 1)
                    {
                        day.insert(day.begin(), '0');
                    }

                    Date = year + "-" + month + "-" + day;
                    std::cout << Title << " | " <<  Date << " | " << Time << "\n";
                    db << "INSERT INTO Records (Title, Screening_Date, Screening_Time) VALUES (?,?,?)"
                       << Title 
                       << Date 
                       << Time;
                    
                }
            }     
        }
        std::cout << "\n=========\nRecords in database\n";
        db << "SELECT * FROM Records;" >> [](std::string Title, std::string Date, std::string Time) {
            std::cout << Title << " | " << Date <<  " | " << Time <<"\n";
        };
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}