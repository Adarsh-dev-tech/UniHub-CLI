#include "subjects.h"
#include "storage.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>

namespace uni {

static const std::vector<std::string> kBranches = {
    "CSE","ECE","EEE","ICE","ME","CHE","PROD","CIVIL","MME","ARCH"
};

static std::map<std::string, std::vector<std::string>> subjectPool() {
    return {
        {"CSE", {"Programming Fundamentals","Data Structures","Algorithms","Computer Networks","Operating Systems","DBMS","Software Engineering","AI Basics","ML Intro","Compilers"}},
        {"ECE", {"Circuit Theory","Signals and Systems","Digital Electronics","Communication Systems","Microprocessors","VLSI Basics","Control Systems","Embedded Systems","Antennas","DSP"}},
        {"EEE", {"Electrical Machines","Power Systems","Power Electronics","Control Systems","Measurements","Switchgear","Renewable Energy","HV Engineering","Microgrids","Drives"}},
        {"ICE", {"Sensors","Transducers","Process Control","Industrial Instrumentation","Biomedical","Analytical Instruments","Control Theory","Automation","Robotics","PLC"}},
        {"ME", {"Engineering Mechanics","Thermodynamics","Manufacturing","Fluid Mechanics","Heat Transfer","Design of Machines","IC Engines","Refrigeration","Dynamics","CAD/CAM"}},
        {"CHE", {"Material Balance","Energy Balance","Fluid Operations","Heat Operations","Mass Transfer","Chemical Reaction Engg","Process Control","Plant Design","Bioprocess","Polymer Tech"}},
        {"PROD", {"Foundry","Welding","Metrology","Manufacturing Systems","Operations Research","CIM","Quality Control","Supply Chain","Maintenance","Ergonomics"}},
        {"CIVIL", {"Surveying","Strength of Materials","Structural Analysis","Geotechnical","Transportation","Hydraulics","Environmental","Construction","Irrigation","Estimating"}},
        {"MME", {"Physical Metallurgy","Mechanical Metallurgy","Extractive","Phase Transformations","Materials Characterization","Welding Metallurgy","Powder Metallurgy","Corrosion","Nanomaterials","Heat Treatment"}},
        {"ARCH", {"Design Studio","Building Materials","History of Architecture","Structures","Climatology","Urban Design","Landscape","Housing","Conservation","Professional Practice"}}
    };
}

static std::string teacherFor(const std::string& /*branch*/, int year, int sem, char section, int idx) {
    // Deterministic pseudo teacher names based on inputs
    std::vector<std::string> first = {"A.","B.","C.","D.","E.","F."};
    std::vector<std::string> last = {"Raman","Iyer","Sharma","Gupta","Natarajan","Srinivasan","Kumar","Reddy"};
    int k = (year*13 + sem*7 + (section=='A'?1:2)*11 + idx*5) % (int)last.size();
    return std::string("Prof. ") + last[k] + " (" + (section=='A'?"Sec A":"Sec B") + ")";
}

std::vector<Subject> getSubjects(int year, int semester, const std::string& branch, char section) {
    // Custom curriculum override: CSE Year-2 Semester-3 Section-B
    if ((branch == "CSE") && (year == 2) && (semester == 3) && (section == 'B' || section == 'b')) {
        std::vector<Subject> out;
        const std::vector<std::pair<std::string,std::string>> custom = {
            {"computer organization", "Prof. Mala"},
            {"principles of programming languages", "Prof. Bala"},
            {"Data Structures", "Prof. Oswald"},
            {"Probability and operations research", "Prof. Shivaranjini"},
            {"Combinatorics and graph theory", "Prof. Pavan"},
            {"digital systems design", "Prof. Shameedha"}
        };
        for (int i = 0; i < (int)custom.size(); ++i) {
            Subject s;
            s.code = branch + std::to_string(year) + std::to_string(semester) + char('A' + i); // e.g., CSE23A..F
            s.name = custom[i].first;
            s.teacher = custom[i].second;
            out.push_back(std::move(s));
        }
        return out;
    }

    auto pool = subjectPool();
    std::vector<Subject> out;
    auto it = pool.find(branch);
    if (it == pool.end()) return out;
    const auto& names = it->second;
    // choose 5 subjects per semester
    for (int i = 0; i < 5 && i < (int)names.size(); ++i) {
        Subject s;
        s.code = branch + std::to_string(year) + std::to_string(semester) + char('A'+i);
        s.name = names[(semester*3 + i) % names.size()];
        s.teacher = teacherFor(branch, year, semester, section, i);
        out.push_back(s);
    }
    return out;
}

std::string resourcesBase(int year, int semester, const std::string& branch, char section, const std::string& subjectName, const std::string& type) {
    std::string base = resourcesDir() + "/" + std::to_string(year) + "/" + std::to_string(semester) + "/" + branch + "/" + section + "/" + subjectName + "/" + type;
    ensureDir(base);
    return base;
}

}
