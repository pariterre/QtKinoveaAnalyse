#include "KinoveaReader.h"

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
    tinyxml2::XMLNode * root(FirstChildElementProtected(FirstChildElementProtected(FirstChildElementProtected(&xml, "Workbook"), "Worksheet"), "Table"));
    tinyxml2::XMLNode * row;
    tinyxml2::XMLNode * col;
    tinyxml2::XMLNode * cell;
    tinyxml2::XMLNode * valueNode;

    std::vector<Frame> frames;
    for (ProportionalModel::Landmark landmark : model.GetLandmarks()){
        bool isLandmarkFound(false);
        Frame f;
        row = FirstChildElementProtected(root, "Row");

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
                    frames.clear();
                    // Skip 2 rows (the third is skiped by the following continue
                    for (int i=0; i<2; ++i){
                        row = row->NextSiblingElement("Row");
                        if (!row) // If we are at the end of the file
                            break;
                    }
                    std::cout << landmark.GetName() << std::endl;
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
            double y(std::stod(valueNode->ToText()->Value()));

            // Get X value
            col = col->NextSiblingElement("Cell");
            cell = col->FirstChildElement("Data");
            if (!cell)
                break; // Unkown error... if it ever happens
            valueNode = cell->FirstChild();
            if (!valueNode) // End of the landmark values
                break;
            double x(std::stod(valueNode->ToText()->Value()));

            // Get Time value
            col = col->NextSiblingElement("Cell");
            cell = col->FirstChildElement("Data");
            if (!cell)
                break; // Unkown error... if it ever happens
            valueNode = cell->FirstChild();
            if (!valueNode) // End of the landmark values
                break;
            double t(parseTime(valueNode->ToText()->Value()));

            f.SetTime(t);
            f.SetX(x);
            f.SetY(y);
            frames.push_back(f);
        }
        _frames.push_back(frames);
    }
    if (_frames.size() != model.GetLandmarks().size())
        throw std::runtime_error("All landmarks should appear in the XML file");

    // The user may not have targeted the same frames for each landmark. We have to drop
    // the non-shared frames
    for (size_t currentLandmarkIdx=0; currentLandmarkIdx<_frames.size(); ++currentLandmarkIdx){
        // Look into the time frames of each landmark. If one time frame doesn't exist in
        // at least one of the landmark, remove it from the current landmark.
        for (int currentTimeIdx=static_cast<int>(_frames[currentLandmarkIdx].size()-1); currentTimeIdx>=0; --currentTimeIdx){
            for (size_t comparedLandmarkIdx=0; comparedLandmarkIdx<_frames.size(); ++comparedLandmarkIdx){
                bool timeFound(false);
                for (size_t comparedTimeIdx=0; comparedTimeIdx<_frames[comparedLandmarkIdx].size(); ++comparedTimeIdx){
                    double time1(_frames[currentLandmarkIdx][static_cast<size_t>(currentTimeIdx)].GetTime());
                    double time2( _frames[comparedLandmarkIdx][comparedTimeIdx].GetTime());
                    if ( fabs(time1 - time2) < 1e-10){
                        if (currentLandmarkIdx == comparedLandmarkIdx && currentTimeIdx != static_cast<int>(comparedTimeIdx))
                            // There is a bug in Kinovea where timestamps appear sometime
                            // twice. We must remove them
                            timeFound = false;
                        else
                            timeFound = true;
                        break;
                    }
                }
                if (!timeFound){
                    _frames[currentLandmarkIdx].erase(_frames[currentLandmarkIdx].begin() + currentTimeIdx);
                    break;
                }
            }
        }
    }
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

KinoveaReader::Frame::Frame() :
    _t(-1),
    _x(0),
    _y(0)
{

}

double KinoveaReader::Frame::GetTime() const
{
    return _t;
}

void KinoveaReader::Frame::SetTime(double t)
{
    _t = t;
}

double KinoveaReader::Frame::GetX() const
{
    return _x;
}

void KinoveaReader::Frame::SetX(double x)
{
    _x = x;
}

double KinoveaReader::Frame::GetY() const
{
    return _y;
}

void KinoveaReader::Frame::SetY(double y)
{
    _y =  y;
}
