/*
 * csv2fvec.cc
 *
 *
 *
 */


#include "csv2fvec.hh"
#include <TSQLServer.h>
#include <TSQLResult.h>
#include <TSQLRow.h>


csv2fvec::csv2fvec()
{
}

std::vector<std::vector<float>> csv2fvec::fromDatacard (std::string datacardname)
{

const char* getenvdatacard=std::getenv ("DATACARDS");
    std::string datacarddir;
    if (getenvdatacard) {
        datacarddir=getenvdatacard;
    } else {
        std::cerr << "CSV2FVEC: Warning - environment variable DATACARDS empty; switching to default = ../data" << std::endl;
        datacarddir = "../data";
    }
    return fromFile (datacarddir + "/" + datacardname);
}

std::vector<std::vector<float>> csv2fvec::fromFile (std::string filename)
{
    std::vector<std::vector<float>> entries;
    std::ifstream fp (filename);
    if (!fp.good() ) {
        std::cout << "CSV2FVEC: failed loading " << filename << std::endl;
        return entries;
    }
    std::string line;
    while (std::getline (fp, line, '\n') ) {
        float cell;
        std::istringstream row (line);
        std::vector<float> vline;
        while (row >> cell) {
            vline.push_back (cell);
            if (row.peek() == ',') row.ignore();
        }
        entries.push_back (vline);
    }
    std::cout << "CSV2FVEC: success loading " << filename << " ("  << entries.size() << " entries)" << std::endl;
    return entries;
}



std::vector<std::vector<float>> fromSQL (std::string filename, std::string sqlrequest)  // put later defaults, add username?
{
    std::vector<std::vector<float>> entries;
    // Mostly copied from https://root.cern.ch/doc/master/sqlselect_8C.html
    TSQLServer *db = TSQLServer::Connect (filename.c_str(), "nobody", "");
    //printf("Server info: %s\n", db->ServerInfo());

    TSQLResult* res = db->Query (sqlrequest.c_str());
    int nrows = res->GetRowCount();
    int nfields = res->GetFieldCount();
    for (int irow = 0; irow < nrows; irow++) {
        TSQLRow *row = res->Next();
        std::vector<float> vline;
        for (int ifield = 0; ifield < nfields; ifield++) {
              std::string sfield(row->GetField (ifield));
              float ffield=std::stof(sfield);
              vline.push_back (ffield);
        }

        entries.push_back (vline);
        delete row;
    }
    delete res;
    delete db;
    return entries;
}
