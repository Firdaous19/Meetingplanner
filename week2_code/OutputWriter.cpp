//
// Created by firdi on 21/04/2026.
//
#include "OutputWriter.h"
#include <fstream>
#include <iostream>

std::string OutputWriter::formatDate(const std::string& isoDate) const {
    if (isoDate.size() != 10) {
        return isoDate;
    }

    std::string year = isoDate.substr(0, 4);
    std::string month = isoDate.substr(5, 2);
    std::string day = isoDate.substr(8, 2);

    return day + "/" + month + "/" + year;
}

void OutputWriter::writeOutput(const std::string& filename, const MeetingPlanner& planner) const {
    std::ofstream out(filename.c_str());

    if (!out.is_open()) {
        std::cerr << "Fout: outputbestand kon niet gemaakt worden." << std::endl;
        return;
    }

    out << "Past meetings:\n";

    out << "\nFuture meetings:\n";
    for (const auto& meeting : planner.getSuccessfulMeetings()) {
        out << "- " << meeting.getRoomIdentifier()
            << ", " << formatDate(meeting.getDate()) << "\n";
        out << meeting.getLabel() << "\n";

        const auto& participants = meeting.getParticipants();
        for (size_t i = 0; i < participants.size(); i++) {
            out << participants[i];
            if (i < participants.size() - 1) {
                out << ", ";
            }
        }
        out << "\n";
        out << "Meeting ID: " << meeting.getIdentifier() << "\n";
    }

    out << "\nConflicts:\n";
    for (const auto& conflict : planner.getConflicts()) {
        out << "- " << conflict << "\n";
    }

    out << "\nRooms:\n";
    for (const auto& room : planner.getRooms()) {
        out << "- " << room.getIdentifier() << "\n";
        out << "Capacity: " << room.getCapacity() << " people\n";
    }

    out.close();
}