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

    out << "## ==== [SYSTEM STATUS] ==== ##\n\n";

    out << "--== Meetings ==--\n";
    for (const auto& meeting : planner.getSuccessfulMeetings()) {
        out << "[" << meeting.getIdentifier() << "]\n";
        out << "- Label: " << meeting.getLabel() << "\n";
        out << "- Time: " << formatDate(meeting.getDate()) << "\n";

        if (meeting.isOnline()) {
            out << "- Location: online\n";
        } else {
            out << "- Location: " << meeting.getRoomIdentifier() << "\n";
        }

        out << "- Participants: ";
        const auto& participants = meeting.getParticipants();
        for (size_t i = 0; i < participants.size(); i++) {
            out << participants[i];
            if (i < participants.size() - 1) {
                out << ", ";
            }
        }
        out << "\n";

        out << "- Room occupancy: "
            << meeting.getOccupancyPercentage()
            << "%\n";

        if (meeting.areExternalsAllowed()) {
            out << "- Externals allowed\n";
        } else {
            out << "- Externals not allowed\n";
        }

        if (meeting.hasCatering()) {
            out << "- Catering\n";
        } else {
            out << "- No catering\n";
        }

        out << "- CO2 emitted: " << meeting.getCO2Emission() << "g\n\n";
    }

    out << "--== Rooms ==--\n";
    for (const auto& room : planner.getRooms()) {
        out << "[" << room.getIdentifier() << "]\n";
        out << "- Name: " << room.getName() << "\n";
        out << "- Capacity: " << room.getCapacity() << "\n";
        out << "- Occupied: " << (room.isOccupied() ? "yes" : "no") << "\n\n";
    }

    out << "--== Conflicts ==--\n";
    if (planner.getConflicts().empty()) {
        out << "No conflicts\n\n";
    } else {
        for (const auto& conflict : planner.getConflicts()) {
            out << "- " << conflict << "\n";
        }
        out << "\n";
    }

    int totalCO2 = 0;
    for (const auto& meeting : planner.getSuccessfulMeetings()) {
        totalCO2 += meeting.getCO2Emission();
    }

    out << "--== CO2 summary ==--\n";
    out << "- Total CO2 emitted: " << totalCO2 << "g\n";

    out.close();
}