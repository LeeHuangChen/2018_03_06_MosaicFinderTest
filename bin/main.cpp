#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cfloat>
#include <algorithm>
#include <iterator>
#include <boost/program_options.hpp>
#include "inputGraph.h"
using namespace boost;
using namespace std;
namespace po = boost::program_options;

template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
  copy(v.begin(), v.end(), ostream_iterator<T>(cout, " ")); 
  os.flush();
  return os;
}

int main (int ac, char * av[]) {  
  try 
  {
    float pident,pidentStrict,evalue,evalueStrict;
    string outputFile;
    ofstream filestr;
    streambuf *backup = 0;
    po::options_description desc("Allowed options");
    
    desc.add_options()
    ("help,h", "produce help message")
    ("input-file,i", po::value< vector<string> >(), "input files list")
    ("output-file,o", po::value<string>(&outputFile)->default_value("stdout"),"output file")
    ("evalue,e", po::value<float>(&evalue)->default_value(10),"evalue")
    ("pident,p", po::value<float>(&pident)->default_value(0), "pident")
    ("evalueStrict,E", po::value<float>(&evalueStrict)->default_value(10), 
     "Strict evalue")
    ("pidentStrict,P", po::value<float>(&pidentStrict)->default_value(0), 
     "Strict pident")
    ;
    
    po::positional_options_description p;
    p.add("input-file", -1);
    
    po::variables_map vm;
    po::store(po::command_line_parser(ac, av).
              options(desc).positional(p).run(), vm);
    po::notify(vm);
    
    if (vm.count("help")) 
    {
      cerr << "Usage: options_description [options]\n";
      cerr << desc;
      return 0;
    }
    
    if ( !vm.count("input-file"))
    {
      cerr << "No Input file choosen"<< "\n";
      return 0;
    }
    
    if ( (10 - evalue) > FLT_EPSILON )
      cerr << "evalue threshold is " << evalue << "\n";
    
    if ( pident  > FLT_EPSILON ) 
      cerr << "pident threshold is " << pident << "\n";

    if ( 10 - evalueStrict > FLT_EPSILON )
      cerr << "evalueStrict threshold is " << evalueStrict << "\n";

    if ( pidentStrict > FLT_EPSILON ) 
      cerr << "pidentStrict threshold is " << pidentStrict << "\n";

    if (outputFile != "stdout")
    {
      streambuf *psbuf;
      filestr.open (outputFile.c_str());
      if(filestr.is_open())
      {
        backup = std::cout.rdbuf();
        psbuf = filestr.rdbuf();        // get file's streambuf
        cout.rdbuf(psbuf);
      }
    }
	  
    for (int i =0; i < (int)(vm["input-file"].as< vector<string> >()).size(); ++i) 
    {
      string filename;
      filename = vm["input-file"].as< vector<string> >()[i];
      cerr << "Input files : " << filename <<endl;
      InputGraph input (filename, evalue, pident,evalueStrict,pidentStrict);
      input.execute(evalueStrict,pidentStrict);
    }
    
    if (outputFile != "stdout" && filestr.is_open())
      filestr.close();
  
    cout.rdbuf(backup);
  }
  catch(std::exception& e)
  {
    cout << e.what() << "\n";
    return 1;
  }    
  catch (std::string const& exeption) 
  {
    cout << exeption << std::endl;
    return 1;
  }
  return 0;
}
