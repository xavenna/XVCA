#include "assembler-util.h"




int rewriteNum(std::string n) {
  if(n.substr(0,1) == "$") {
    //number is hex
    return std::stoi (n.substr(1),nullptr,16);
  }
  else if(n.substr(0,1) == "%") {
    //number is binary
    return std::stoi (n.substr(1),nullptr,2);
  }
  else {
    //number is decimal
    return std::stoi (n);
  }
}
bool isAsmNum(const std::string& n) {
  if(!n.empty()) {
    if(n[0] == '$') {
      return isNum(n.substr(1), 16);
    }
    if(n[0] == '%') {
      return isNum(n.substr(1), 2);
    }
  }
  return false;
}

void createHeader(std::vector<char>& machineCode, std::vector<char>& machineCodeWithHeader, uint16_t jp) {
  //std::cout << "jp: " << jp << '\n';
  char jpH = ((jp & 0xff00) >> 8) & 0xff;
  char jpL = jp & 0xff;
  char temp[16]{'X','V','C','A',0x10,0x00, jpH, jpL, '\0','\0','\0','\0','\0','\0','\0','\0'};
  machineCodeWithHeader.resize(16 + machineCode.size());
  memcpy(&(*machineCodeWithHeader.begin()), temp, 16);  // this is extremely janky and probably unsafe
  memcpy(&(*(machineCodeWithHeader.begin()+16)), &(*machineCode.begin()), machineCode.size());

}

/* XMLL Constructs:
 *
 *  for(register, startval, endval, increment)
 *    ; commands
 *  endfor
 *
 *  NOTE - A is not permitted as the register used
 *  a for loop - transformed into the following:
 *
 *  ;;; (for)
 *    MVV[reg] startval ;; if startval is a register, use MVR[reg] instead
 *  xmllBeginFor[unique_id]:
 *    MVVA [endval]
 *    CMP [reg]
 *    JZ xmllEndFor[unique_id]
 *
 *
 *    ; commands
 *
 *  ;;; (endfor)
 *    MVVA [increment]
 *    ADD [reg]  ;;maybe this should be ADC?
 *    MVR[reg] A
 *    JMP xmllBeginFor[unique_id]
 *  xmllEndFor[unique_id]:
 *
 *
 *
 *
 */

bool preprocess(const std::string& infile, const std::string& outfile) {
  //go through the code, look for advanced constructs
  //remove comments, handle includes?
  //if, endif, syscall, string expanders
  //how should this work? first parse it into a vector of lines,
  std::ifstream get(infile);
  std::list<std::string> lines;
  std::string line;
  while(get.peek() != EOF) {
    std::getline(get, line);
    if(line.size() <= 1) {
      //line is empty, disregard
      continue;
    }
    //first, prune preceding whitespace.
    while(line.size() >= 1 && isEmpty(line[0])) { 
      //excise indentation for now. it will be added back for lines with no label
      if(line.size() == 1) {
        line.clear();
        break;
      }
      line = line.substr(1);
    } 

    //this is safe because || is a short-circuit operator
    if(line.size() == 0 || line[0] == ';') {
      //line is empty or a comment, disregard
      continue;
    }
    else {
      if(line.size() == 1 && isEmpty(line[0])) {
        continue;
      }
      lines.push_back(line.substr(0, line.find(';')));
    }
  }
  std::deque<forEntry> forListing; //record of each for in program
                                   //do the actual preprocessing
  unsigned i = 0; //loop counter
  for(auto it = lines.begin();it != lines.end();std::advance(it, 1)) {
    auto& x = *it;
    //std::clog << x << '\n';
    if(x.find("syscall(") != std::string::npos) {
      std::cerr << "Error: Syscalls aren't legal yet.\n";
      return false;
    }
    else if(x.find("if(") != std::string::npos) {

    }
    else if(x.find("endif") != std::string::npos) {

    }
    else if(x.find("for(") != std::string::npos) {
      //get string inside parentheses
      auto last(std::prev(it));
      lines.erase(it);
      it = std::next(last);
      auto y = x.find(')');
      auto spos = x.find('(');
      if(y == std::string::npos) {
        std::cerr << "Error: for statement is missing a close-parenthesis\n";
        return false;
      }
      std::string inside = x.substr(spos+1,y-spos-1);
      int increment = 1;
      int startval = 0;
      int endval = 0;
      //parse inside
      //should be for(reg,start,end)
      std::vector<std::string> args;
      parse(inside, args, ",");
      if(args.size() != 3 && args.size() != 4) {
        std::cerr << "Error: for statement has the wrong number of arguments\n";
        return false;
      }
      if(args[0].size() != 1 || !isReg(args[0][0])) {
        std::cerr << "Error: invalid register name in for statement\n";
        return false;
      }
      if(!isAsmNum(args[1]) || !isAsmNum(args[2])) {
        std::cerr << "Error: argument to for loop is not an integer\n";
        return false;
      }
      startval = rewriteNum(args[1]);
      endval = rewriteNum(args[2]);
      if(args.size() == 4) {
        if(!isNum(args[3])) {
          std::cerr << "Error: increment argument in for statement is not an integer\n";
          return false;
        }
        increment = std::stoi(args[3]);
      }
      forEntry t;
      t.reg = args[0][0];
      t.line = i;
      t.increment = increment;
      t.endval = endval;
      forListing.push_back(t);
      //generate intro code
      /*    MVV[reg] startval ;; if startval is a register, use MVR[reg] instead
       *  xmllBeginFor[unique_id]:
       */
      line = std::string("MVV") + t.reg + " " + std::to_string(startval);
      lines.insert(it, line);

      line = std::string("xmllBeginFor")+std::to_string(t.line) + ":";
      lines.insert(it, line);
    }
    else if(x.find("endfor") != std::string::npos) {
      if(forListing.size() == 0) {
        std::cerr << "Error: endfor without preceding for located\n";
        return false;
      }
      auto last(std::prev(it));
      lines.erase(it);
      it = std::next(last);
      forEntry t = forListing.back();
      forListing.pop_back();
      /*
       *    MVVA [increment]
       *    ADR [reg]  ;;maybe this should be ADC?
       *    MVR[reg] A
       *
       *    MVVA [endval]
       *    CMPR [reg]
       *    JZ xmllEndFor[unique_id]
       */
      line = std::string("MVVA ") + std::to_string(t.increment);
      lines.insert(it, line);

      line = std::string("ADR ") + t.reg;
      lines.insert(it, line);

      line = std::string("MVR") + t.reg + " A";
      lines.insert(it, line);

      line = std::string("MVVA ") + std::to_string(t.endval);
      lines.insert(it, line);

      line = std::string("CMPR ") + t.reg;
      lines.insert(it, line);

      line = std::string("JNZ xmllBeginFor")+std::to_string(t.line);
      lines.insert(it, line);
    }
    else if(x.find("str(") != std::string::npos) {
      //add a string constant
    }
    i++;
  }
  //write the file to outfile
  std::ofstream put(outfile);
  for(auto x : lines) {
    if(x.find(':') == std::string::npos) {
      put << '\t';
    }
    put << x << '\n';
  }
  return true;
}


bool writeMachineCodeToFile(std::vector<char>& machineCode, std::string filename) {
  size_t len = machineCode.size();
  std::ofstream put(filename, std::ios::trunc | std::ios::binary);
  if(!put.is_open()) {
    return false;
  }
  char* buffer = new char[len];
  memcpy(buffer, &(*machineCode.begin()), len);
  put.write(buffer, len);
  delete[] buffer;
  return true;
}

bool fixLabelJumpPoints(std::vector<char>& machineCode, std::map<std::string, int>& labelHash, std::map<int, std::string> jumpTable, std::vector<int> codesPerLine, const unsigned offset) {
  //machineCode contains the unfinished machine code for the program.
  //labelHash contains a list of which line of code each label corresponds to
  //jumpTable contains a list of where a label is needed (in machine code), and which label is needed there
  //codesPerLine tells how many bytes of machine code each line was translated into


  //first, construct a list of the machine code positions of each line of code
  std::vector<int> cumulativePosition;  // machine code position of each line of code
  int accum = 0;
  for(auto x : codesPerLine) {  //make sure this actually works
    cumulativePosition.push_back(accum);
    accum += x;
  }
  cumulativePosition.push_back(accum);

  //second, create a list of where each label is located in machine code, rather than lines of code
  for(auto& x : labelHash) {
    x.second = cumulativePosition.at(x.second);
  }

  //next, iterate through jumpTable to find each place where an address is needed.
  //  Find where that label is located in machine code, and write the address to the placeholder space after the instruction
  try {
    for(auto x : jumpTable) {
      //x.second is label name
      //labelHash[x.second] is label position
      //add label position to starting offset, and write to machineCode[x.first]
      if(labelHash.find(x.second) == labelHash.end()) {
        std::cout << "Error: Attempt to jump to nonexistant label '"<<x.second<<"'.\n";
        return false;
      }
      int relativeAddress = labelHash.find(x.second)->second;  //add exception handling here, as an undefined label will currently cause a crash
      int absoluteAddress = relativeAddress + offset;  //offset is the location of the first byte of machine code and thus the location of the first byte of machine code
      machineCode[x.first] = (absoluteAddress & 0xff00) >> 8;
      machineCode[x.first+1] = absoluteAddress & 0xff;

      if(labelHash.find(x.second) == labelHash.end()) {
        std::cout << "Error: couldn't find label\n";
      }
    }
  }
  catch (...) {
    std::cout << "Error: invalid label. (or a bug. If you know you didn't make any label mistakes, please contact the developer)\n";
    return false;
  }

  //I think that is all
  return true;
} 

bool isReg(char code) {
  switch(code) {
    case 'A':
    case 'B':
    case 'C':
    case 'X':
    case 'Y':
      return true;
    default:
      return false;
  }
}

char regToNum(char reg) {
  std::string n;
  switch(reg) {
    case 'A':
      return 0x0;
    case 'B':
      return 0x1;
    case 'C':
      return 0x2;
    case 'X':
      return 0x3;
    case 'Y':
      return 0x4;
    default:
      n = "regToNum(): Invalid register " + reg;
      throw std::invalid_argument(n);
  }
}
