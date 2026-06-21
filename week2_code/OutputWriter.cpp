#include "OutputWriter.h"
#include "DesignByContract.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstddef>

std::string OutputWriter::formatDate(const std::string& isoDate) const {
    REQUIRE(!isoDate.empty(), "Datum mag niet leeg zijn");

    if (isoDate.size() != 10) {
        return isoDate;
    }

    std::string year  = isoDate.substr(0, 4);
    std::string month = isoDate.substr(5, 2);
    std::string day   = isoDate.substr(8, 2);
    std::string result = day + "/" + month + "/" + year;

    ENSURE(!result.empty(), "Geformatteerde datum mag niet leeg zijn");

    return result;
    return day + "/" + month + "/" + year;
}

void OutputWriter::writeOutput(const std::string& filename,
                               const MeetingPlanner& planner) const {
    REQUIRE(!filename.empty(), "Output filename mag niet leeg zijn");

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
        const std::vector<std::string>& participants = meeting.getParticipants();
        for (size_t i = 0; i < participants.size(); i++) {
            out << participants[i];
            if (i + 1 < participants.size()) {
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
            out << std::fixed << std::setprecision(2);
            out << "- Catering cost: EUR " << meeting.getCateringCost() << "\n";
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

    out << "--== CO2 summary ==--\n";
    out << "- Total CO2 emitted: " << planner.getTotalCO2Emission() << "g\n";

    out << std::fixed << std::setprecision(2);
    out << "- Total catering cost: EUR " << planner.getTotalCateringCost() << "\n";

    ENSURE(out.good(), "Outputbestand moet correct geschreven zijn");

    out.close();
}