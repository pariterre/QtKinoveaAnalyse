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

    size_t nbLandmarksFound(0);
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
                    nbLandmarksFound++;
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
            std::tm t = {};
            //            std::istringstream ss(valueNode->ToText()->Value());
            std::istringstream ss("23:12:34:01");
//            ss >> std::get_time(&t, "%H:%M:%S:%f");
            ss >> std::get_time(&t, "%H:%M:%S");

            std::cout << "time = " << std::put_time(&t, "%c") << "; x = " << x << "; y = "<< y << std::endl;

            // data_to_stack[0, 0] = datetime.datetime.timestamp(
            //                    datetime.datetime.strptime(root[i+3][2][0].text, "%H:%M:%S:%f")
            //                )
            //                data_to_stack[1, 0] = root[i+3][0][0].text
            //                data_to_stack[2, 0] = root[i+3][1][0].text
            //                data_tp = np.hstack((data_tp, data_to_stack))
        }
        if (nbLandmarksFound != model.GetLandmarks().size())
            throw std::runtime_error("All landmarks should appear in the XML file");
    }

//        // Find the data for each reperes
//        for i in range(len(root)):
//
//            if not repere_found:
//                if root[i][0][0].text is not None and len(root[i]) > 1 and root[i][1][0].text == repere:
//                    repere_found = True
//            // If we finished the repere
//            elif len(root) <= i+3 or root[i+3][0][0].text is None:
//                break
//            // Otherwise add data
//            else:
//                data_to_stack[0, 0] = datetime.datetime.timestamp(
//                    datetime.datetime.strptime(root[i+3][2][0].text, "%H:%M:%S:%f")
//                )
//                data_to_stack[1, 0] = root[i+3][0][0].text
//                data_to_stack[2, 0] = root[i+3][1][0].text
//                data_tp = np.hstack((data_tp, data_to_stack))
//        data[repere] = data_tp  // Save the data

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
