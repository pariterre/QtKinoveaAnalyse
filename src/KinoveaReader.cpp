#include <KinoveaReader.h>

#include <tinyxml2.h>
#include <Landmark.h>
#include <Frame.h>
#include <Point2d.h>
#include <ProportionalModel.h>
#include <Joint.h>

KinoveaReader::KinoveaReader()
{

}

void KinoveaReader::readXml(const std::string &){
    throw std::invalid_argument("KinoveaReader must be called with a Proportional Model");
}

void KinoveaReader::readXml(const std::string &path, const ProportionalModel& model)
{
    tinyxml2::XMLDocument xml;
    xml.LoadFile(path.c_str());
    tinyxml2::XMLElement * workbook = xml.FirstChildElement("Workbook");
    if (!workbook)
        throw std::ios_base::failure("ProportionalModel tag not found");

    tinyxml2::XMLElement * root(FirstChildElementProtected(FirstChildElementProtected(workbook, "Worksheet"), "Table"));
    tinyxml2::XMLElement * row;
    tinyxml2::XMLElement * col;
    tinyxml2::XMLElement * cell;
    tinyxml2::XMLNode * valueNode;

    size_t nbLandmarks(model.GetLandmarks().size());
    _frames.clear();
    for (size_t landmarkIdx = 0; landmarkIdx < nbLandmarks; ++ landmarkIdx){
        row = FirstChildElementProtected(root, "Row");
        const Landmark& landmark = model.GetLandmarks()[landmarkIdx];

        bool isLandmarkFound(false);
        size_t frameIdx(0);
        while (true){
            row = row->NextSiblingElement("Row");
            if (!row)
                break;

            // If we haven't reach the landmark so far
            if (!isLandmarkFound){
                // The landmark name is on the second column of the row with "Label :" tag on first col
                col = row->FirstChildElement("Cell");
                if (!col)
                    continue;
                cell = col->FirstChildElement("Data");
                if (!cell)
                    continue;
                valueNode = cell->FirstChild();
                if (!valueNode)
                    continue;
                if (std::string(valueNode->ToText()->Value()).compare("Label :"))
                    continue;
                col = col->NextSiblingElement("Cell");
                if (!col)
                    continue;
                cell = col->FirstChildElement("Data");
                if (!cell)
                    continue;
                if (!FirstChildToTextValueProtected(cell).compare(landmark.GetName())){
                    isLandmarkFound = true;
                    // Skip 2 rows (the third is skiped by the following continue
                    for (int i=0; i<2; ++i){
                        row = row->NextSiblingElement("Row");
                        if (!row) // If we are at the end of the file
                            break;
                    }
                }
                continue; // Go to the next row
            }
            // Reminder, if we get here, isLandmarkFound is true
            if (!row) // If we are at the end of the file (carry the information)
                break;

            // Get Y value
            col = row->FirstChildElement("Cell");
            if (!col)
                break; // EOF
            cell = col->FirstChildElement("Data");
            if (!cell)
                break; // Unkown error... if it ever happens
            valueNode = cell->FirstChild();
            if (!valueNode) // End of the landmark values
                break;
            double x(std::stod(valueNode->ToText()->Value()));

            // Get X value
            col = col->NextSiblingElement("Cell");
            cell = col->FirstChildElement("Data");
            if (!cell)
                break; // Unkown error... if it ever happens
            valueNode = cell->FirstChild();
            if (!valueNode) // End of the landmark values
                break;
            double y(std::stod(valueNode->ToText()->Value()));

            // Get Time value
            col = col->NextSiblingElement("Cell");
            cell = col->FirstChildElement("Data");
            if (!cell)
                break; // Unkown error... if it ever happens
            valueNode = cell->FirstChild();
            if (!valueNode) // End of the landmark values
                break;
            double t(parseTime(valueNode->ToText()->Value()));

            // Find the frame with the timestamp
            size_t idx(UINT_MAX);
            for (size_t i = 0; i<_frames.size(); ++i){
                if (t <= _frames[i].GetTime()){
                    idx = i;
                    break;
                }
            }
            if (idx == UINT_MAX){ // If we didn't find the frame, append a new one
                Frame f(nbLandmarks, 0);
                f.SetTime(t);
                _frames.push_back(f);
                idx = _frames.size() - 1;
            } else if (t < _frames[idx].GetTime()) { // If it is smaller than the actual frame, insert a new one there
                Frame f(nbLandmarks, 0);
                f.SetTime(t);
                _frames.insert(_frames.begin() + static_cast<int>(idx), f);
            }

            // Note there is a bug in Kinovea where timestamps appear sometime
            // twice. By design, this algorithm removes them
            _frames.at(idx).SetPoint2d(landmarkIdx, Point2d(x/100, y/100, landmark.GetName())); // /100 for cm => m
            ++frameIdx;
        }
    }

    // The user may not have targeted the same frames for each landmark. We have to drop
    // the non-shared frames.
    // And
    for (size_t i = _frames.size() - 1; i < _frames.size(); --i)
        if (!_frames[i].isAllSegmentsAreSet())
            _frames.erase(_frames.begin() + static_cast<int>(i));

}

const std::vector<Frame>& KinoveaReader::GetFrames() const
{
    return _frames;
}

double KinoveaReader::parseTime(const std::string &time)
{
    size_t idxH(0); // index hour
    size_t idxM(time.find_first_of(":", idxH + 1)); // index minutes
    size_t idxS(time.find_first_of(":", idxM + 1)); // index seconds
    size_t idxF(time.find_first_of(":", idxS + 1)); // index fraction of second

    double hour(std::stod(time.substr(idxH, idxM - idxH)));
    double min(std::stod(time.substr(idxM + 1, idxS - idxM - 1)));
    double sec(std::stod(time.substr(idxS + 1, idxF - idxS - 1)));
    double frac(std::stod(time.substr(idxF + 1)));

    double totalSecond(0);
    totalSecond += frac / static_cast<double>( std::pow(10, time.substr(idxF + 1).size()) );
    totalSecond += sec;
    totalSecond += min * 60.0;
    totalSecond += hour * 3600.0;

    return totalSecond;
}

