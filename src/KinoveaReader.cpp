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


            // data_to_stack[0, 0] = datetime.datetime.timestamp(
            //                    datetime.datetime.strptime(root[i+3][2][0].text, "%H:%M:%S:%f")
            //                )
            //                data_to_stack[1, 0] = root[i+3][0][0].text
            //                data_to_stack[2, 0] = root[i+3][1][0].text
            //                data_tp = np.hstack((data_tp, data_to_stack))
        }
        _frames.push_back(frames);
    }
    if (_frames.size() != model.GetLandmarks().size())
        throw std::runtime_error("All landmarks should appear in the XML file");

//    // Find share timestamps
//    // Arbitrarily take the time of first repere as time reference
//    shared_time = data[reperes_anato[0]][0, :]
//    shared_time = np.unique(shared_time)
//    for d in data.values():
//        shared_time_tp = d[0, np.isin(d[0, :], shared_time)]
//        shared_time = shared_time[np.isin(shared_time, shared_time_tp)]

//    // Remove unshared timestamps
//    for (k, d) in data.items():
//        idx = np.isin(d[0, :], shared_time)
//        _, idx = np.unique(d[0, idx], return_index=True)
//        data[k] = d[1:3, idx] * 0.01  // From cm to m

//    // Return data
//    time = shared_time - shared_time[0]
//    return data, time

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
