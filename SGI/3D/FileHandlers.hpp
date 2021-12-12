#ifndef FILEHANDLERS_HPP
#define FILEHANDLERS_HPP

#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>
#include <string>

/*
    Possiveis Diretivas:
        v, o, p, l, f, curv, surf,
        bsp, bzp, cstype, deg, end
        usemtl, newmtl, Kd
    Obs:
        Index dos vertices podem ser positivos ou negativos.
        Os arquivos .mtl devem estar na mesma pasta que o arquivo .obj.
        Pode-se usar '\' na declaração dos vertices dos objetos.
*/

class ObjStream
{
    public:
        ObjStream(std::string& filename);
    protected:
        std::string _name;
        std::string _path;
        std::fstream _objsFile;
        std::fstream _colorsFile;
        GdkRGBA _color{};
};

class ObjReader : public ObjStream
{
    public:
        ObjReader(std::string& filename);
        std::vector<Objeto*>& getObjs(){ return _objs; }
    private:
        void loadObjs();
        void setName(std::stringstream& line);
        void loadColorsFile(std::stringstream& line);
        void changeColor(std::stringstream& line);
        void addCoord(std::stringstream& line);
        void addPonto(std::stringstream& line);
        void addPoligono(std::stringstream& line, bool filled = false);
        void addFace(std::stringstream& line);
        void addCurva(std::stringstream& line);
        void addSuperficie(std::stringstream& line, ObjType type);
        void addObj3D();
        void loadIndexesCoords(std::stringstream& line, Coordinates& objCoords);
        void destroyObjs();
        void setTipoFormaLivre(std::stringstream& line);
    private:
        std::vector<Objeto*> _objs;
        Coordinates _coords;
        int _numSubObjs = 0;
        ObjType _tipoFormaLivre = ObjType::OBJETO;
        std::string _faceName = "";
        ListaFaces _faces;
};

class ObjWriter : public ObjStream
{
    public:
        ObjWriter(std::string& filename);
        void writeObjs(World *world);
    private:
        void printObj(Objeto* obj);
        void printObj3D(Objeto3D* obj);
        void printCurva(Curva2* obj);
        void printSuperficie(SuperficieBezier* obj);
        void printSuperficie(SuperficieBSpline* obj);
    private:
        int _numVertex = 0;
};

ObjStream::ObjStream(std::string& filename) 
{
    if(!filename.find("\\.obj"))
        filename += ".obj";
    std::size_t found = filename.find_last_of("/\\");
    _path = filename.substr(0,found+1);
    _name = filename.substr(found+1, filename.size()-found-5);
}

ObjReader::ObjReader(std::string& filename): ObjStream(filename)
{
    _objsFile.open(filename.c_str());
    if(!_objsFile.is_open())
        throw ("Erro tentando abrir o arquivo:\n\t" + filename + ".\n");
    else
        loadObjs();
}

void ObjReader::destroyObjs()
{
    for(auto o : _objs)
        delete o;
}

void ObjReader::addObj3D()
{
    std::string name = _numSubObjs == 0 ? _name : _name+"_sub"+std::to_string(_numSubObjs);
    _objs.push_back(new Objeto3D(name, _faces));
    _faces.clear();
}

void ObjReader::loadObjs()
{
    std::string tmp, keyWord;
    while(std::getline(_objsFile, tmp))
    {
        if(tmp.size() <= 1) continue;
        std::stringstream line(tmp);
        line >> keyWord;
        if(keyWord == "#")              { /* ... */ }
        else if(keyWord == "v")         { addCoord(line); }
        else if(keyWord == "o")         { setName(line); }
        else if(keyWord == "w")         { /* TODO */ }
        else if(keyWord == "p")         { addPonto(line); }
        else if(keyWord == "l")         { addPoligono(line, false); }
        else if(keyWord == "f")         { addFace(line); }
        else if(keyWord == "curv")      { addCurva(line); }
        else if(keyWord == "surf")      { addSuperficie(line,ObjType::OBJETO); }
        else if(keyWord == "bzp")       { addSuperficie(line,ObjType::SUPERFICIE_BEZIER); }
        else if(keyWord == "bsp")       { addSuperficie(line,ObjType::SUPERFICIE_BSPLINE); }
        else if(keyWord == "cstype")    { setTipoFormaLivre(line); }
        else if(keyWord == "end")       { /* ??? */ }
        else if(keyWord == "deg")       { /* ... */ }
        else if(keyWord == "g")         { /* ... */ }
        else if(keyWord == "vt")        { /* ... */ }
        else if(keyWord == "vn")        { /* ... */ }
        else if(keyWord == "vp")        { /* ... */ }
    }
    if(_faces.size() != 0)
        addObj3D();
}

void ObjReader::setName(std::stringstream& line)
{
    // Caso o nome do objeto mude, deve-se checar
    //  se ja foi carregado alguma Face. Caso tenha sido,
    //  deve-se então criar o objeto 3D com as Faces
    //  carregadas até agora
    if(_faces.size() != 0)
        addObj3D();
    line >> _name;
    _numSubObjs = 0;
}

void ObjReader::addCoord(std::stringstream& line)
{
    double x=0.0,y=0.0,z=0.0;
    line >> x >> y >> z;
    _coords.emplace_back(x,y,z);
}

void ObjReader::addPonto(std::stringstream& line)
{
    if(_faces.size() != 0)
        addObj3D();
    Coordinates objCoords;
    loadIndexesCoords(line, objCoords);
    std::string name = _numSubObjs == 0 ? _name : _name+"_sub"+std::to_string(_numSubObjs);

    for(auto &c : objCoords)
    {
        _objs.push_back(new Ponto(name, c));
        _numSubObjs++;
        name = _name+"_sub"+std::to_string(_numSubObjs);
    }
}

void ObjReader::addPoligono(std::stringstream& line, bool filled)
{
    if(_faces.size() != 0)
        addObj3D();
    Coordinates objCoords;
    loadIndexesCoords(line, objCoords);
    std::string name = _numSubObjs == 0 ? _name : _name+"_sub"+std::to_string(_numSubObjs);
    if(objCoords.size() == 2)
        _objs.push_back(new Reta(name, objCoords));
    else
        _objs.push_back(new Poligono(name, filled, objCoords));
    _numSubObjs++;
}

void ObjReader::addFace(std::stringstream& line)
{
    Coordinates objCoords;
    loadIndexesCoords(line, objCoords);
    std::string name = "face"+std::to_string(_faces.size()+1);
    _faces.emplace_back(name, objCoords);
}

void ObjReader::addCurva(std::stringstream& line)
{
    if(_tipoFormaLivre == ObjType::OBJETO)
    {
        destroyObjs();
        throw("Tentativa de criar uma curva sem 'cstype' na linha: "+ line.str() +".\n");
    }
    if(_faces.size() != 0)
        addObj3D();
    double tmp=0.0;
    line >> tmp;
    line >> tmp;
    Coordinates objCoords;
    loadIndexesCoords(line, objCoords);
    std::string name = _numSubObjs == 0 ? _name : _name+"_sub"+std::to_string(_numSubObjs);
    if(_tipoFormaLivre == ObjType::CURVABEZIER)
    {
        if(objCoords.size() < 4 || (objCoords.size()-4)%3 != 0)
        {
            destroyObjs();
            throw "A Bézier deve ter a forma de { 4, 7, 10, ... } pontos!";
        }
        _objs.push_back(new CurvaBezier(name, objCoords));
    }
    else if(_tipoFormaLivre == ObjType::CURVABSPLINE)
    {
        if(objCoords.size() < 4)
        {
            destroyObjs();
            throw ("Uma curva B-Spline deve ter no minimo 4 coordenadas.");
        }
        _objs.push_back(new CurvaBSpline(name, objCoords));
    }
    _numSubObjs++;
}

void ObjReader::addSuperficie(std::stringstream& line, ObjType type)
{
    if(type == ObjType::OBJETO)
    {
        if(_tipoFormaLivre == ObjType::OBJETO)
        {
            destroyObjs();
            throw "Tentou criar uma superfície sem 'cstype' na linha: " + line.str() +".\n";
        }
        double tmp=0.0;
        line >> tmp;
        line >> tmp;
        line >> tmp;
        line >> tmp;
    }

    Coordinates objCoords;
    loadIndexesCoords(line, objCoords);
    std::string name = _numSubObjs == 0 ? _name : _name+"_sub"+std::to_string(_numSubObjs);
    if(objCoords.size() < 16)
    {
        destroyObjs();
        throw("Superficies devem ter 16 coordenadas.");
    }
    else if(objCoords.size() > 16)
    {
        destroyObjs();
        throw "Para formar mais do que uma superfócie, declare as superfócies compartilhando os vértices em comum.";
    }
    if(type == ObjType::SUPERFICIE_BEZIER || (type == ObjType::OBJETO && _tipoFormaLivre == ObjType::CURVABEZIER))
    {
        _objs.push_back(new SuperficieBezier(name, objCoords));
    }
    else if(type == ObjType::SUPERFICIE_BSPLINE || (type == ObjType::OBJETO && _tipoFormaLivre == ObjType::CURVABSPLINE))
    {
        _objs.push_back(new SuperficieBSpline(name, objCoords));
    }
    _numSubObjs++;
}

void ObjReader::loadIndexesCoords(std::stringstream& line, Coordinates& objCoords)
{
    std::string pointString;
    int index = 0;
    int size = _coords.size();

    while(true)
    {
        while(line >> pointString)
        {
            std::stringstream point(pointString);
            if(point.str() == "\\")
                continue;
            point >> index;
            if(!point)
            {
                destroyObjs();
                throw ("Indice de vertice invalido na linha: "+ line.str() + ".\n");
            }
            if(index < 0)
                index = size + index;
            else
                index--;
            if(index < 0 || index >= size)
            {
                destroyObjs();
                throw("Indice de vertice invalido na linha: "+ line.str() + ".\n");
            }
            objCoords.push_back(_coords[index]);
        }
        if(line.str().find("\\") == std::string::npos)
            break;
        else
        {
            std::string tmp;
            if(std::getline(_objsFile, tmp))
            {
                line.str(tmp);
                line.clear();
            }
            else
                break;
        }
    }
}

void ObjReader::setTipoFormaLivre(std::stringstream& line)
{
    std::string type;
    line >> type;

    if(type == "rat"){ line >> type; }
    if(type == "bezier"){ _tipoFormaLivre = ObjType::CURVABEZIER; }
    else if(type == "bspline"){ _tipoFormaLivre = ObjType::CURVABSPLINE; }
    else
    {
        destroyObjs();
        throw "cstype = '"+ type+"', nao é suportado.\n";
    }
}

ObjWriter::ObjWriter(std::string& filename): ObjStream(filename) 
{
    _objsFile.open(filename.c_str(), std::fstream::out);
    if(!_objsFile.is_open())
        throw "Erro tentando abrir arquivo:\n\t" + filename + ".\n";
}

void ObjWriter::writeObjs(World *world)
{
    _objsFile << "mtllib " << _name << ".mtl\n\n";
    Objeto *obj;
    int size = world->numObjs();
    for(int i = 0; i<size; i++)
    {
        obj = world->getObj(i);
        switch(obj->getType())
        {
            case ObjType::OBJETO3D:
                printObj3D((Objeto3D*) obj);
                break;
            case ObjType::SUPERFICIE_BEZIER:
                printSuperficie((SuperficieBezier*) obj);
                break;
            case ObjType::SUPERFICIE_BSPLINE:
                printSuperficie((SuperficieBSpline*) obj);
                break;
            case ObjType::CURVABEZIER:
            case ObjType::CURVABSPLINE:
                printCurva((Curva2*) obj);
                break;
            default:
                printObj(obj);
        }
    }
}

void ObjWriter::printObj(Objeto* obj)
{
    auto &coords = obj->getCoords();
    for(const auto &c : coords)
        _objsFile << "v " << c.x << " " << c.y << " " << c.z << "\n";
    _objsFile << "\no " << obj->getName() << "\n";

    if(obj->getType() == ObjType::PONTO)
        _objsFile << "p";
    else
        _objsFile << "l";

    int size = coords.size();
    for(int i = 0; i<size; i++)
        _objsFile << " " << _numVertex+(i+1);
    _objsFile << "\n" << std::endl;
    _numVertex += size;
}

void ObjWriter::printObj3D(Objeto3D* obj)
{
    for(auto face : obj->getListaFaces())
    {
        auto &coords = face.getCoords();
        for(const auto &c : coords)
            _objsFile << "v " << c.x << " " << c.y << " " << c.z << "\n";
    }
    _objsFile << "\no " << obj->getName() << "\n";
    unsigned int faceSize = 0;
    for(auto &face : obj->getListaFaces())
    {
        faceSize = face.getCoords().size();
        _objsFile << "f";
        for(unsigned int i = 0; i<faceSize; i++)
        {
            _objsFile << " " << _numVertex+(i+1);
        }
        _objsFile << "\n";
        _numVertex += faceSize;
    }
}

void ObjWriter::printCurva(Curva2* obj)
{
    auto &coords = obj->getPontosControle();
    for(const auto &c : coords)
        _objsFile << "v " << c.x << " " << c.y << " " << c.z << "\n";

    _objsFile << "\no " << obj->getName() << "\n";
    if(obj->getType() == ObjType::CURVABEZIER)
        _objsFile << "cstype bezier\n";
    else
        _objsFile << "cstype bspline\n";
    _objsFile << "deg 3\n";
    _objsFile << "curv 0.0 0.0";
    int size = coords.size();
    for(int i = 0; i<size; i++)
        _objsFile << " " << _numVertex+(i+1);
    _objsFile << "\nend\n" << std::endl;
    _numVertex += size;
}

void ObjWriter::printSuperficie(SuperficieBezier* obj)
{
    for(const auto& c : obj->getPontosControle())
    {
        _objsFile << "v " << c.x << " " << c.y << " " << c.z << "\n";
    }
    _objsFile << "\no " << obj->getName() << "\n";
    _objsFile << "cstype bezier\n";
    _objsFile << "deg 3 3\n";
    int maxLines = obj->getMaxLin(), maxCols = obj->getMaxCols();
    int tmp3xMaxLines = 3*maxLines, tmp3xMaxCols = 3*maxCols;
    for(int nLine = 0; (maxLines != 4 && nLine <= tmp3xMaxLines) || (nLine < tmp3xMaxLines); nLine += tmp3xMaxCols)
    {
        for(int nCol = 0; nCol < maxCols-1; nCol += 3)
        {
            _objsFile << "surf 0.0 1.0 0.0 1.0";
            for(int i = 0; i < 4; i++)
            {
                _objsFile << " " << _numVertex+(i*maxCols)  +nLine+nCol+1;
                _objsFile << " " << _numVertex+(i*maxCols)+1+nLine+nCol+1;
                _objsFile << " " << _numVertex+(i*maxCols)+2+nLine+nCol+1;
                _objsFile << " " << _numVertex+(i*maxCols)+3+nLine+nCol+1;
            }
            _objsFile << "\nend\n\n";
        }
    }
    _numVertex += maxLines*maxCols;
}

void ObjWriter::printSuperficie(SuperficieBSpline* obj)
{
    for(const auto& c : obj->getPontosControle())
    {
        _objsFile << "v " << c.x << " " << c.y << " " << c.z << "\n";
    }

    _objsFile << "\no " << obj->getName() << "\n";
    _objsFile << "cstype bspline\n";
    _objsFile << "deg 3 3\n";
    int maxLines = obj->getMaxLin(), maxCols = obj->getMaxCols();
    for(int nLine = 0; (maxLines != 4 && nLine <= maxLines) || (nLine < maxLines); nLine += maxCols)
    {
        for(int nCol = 0; nCol <= maxCols-4; nCol += 1)
        {
            _objsFile << "surf 0.0 1.0 0.0 1.0";
            for(int i = 0; i < 4; i++)
            {
                _objsFile << " " << _numVertex+(i*maxCols)  +nLine+nCol+1;
                _objsFile << " " << _numVertex+(i*maxCols)+1+nLine+nCol+1;
                _objsFile << " " << _numVertex+(i*maxCols)+2+nLine+nCol+1;
                _objsFile << " " << _numVertex+(i*maxCols)+3+nLine+nCol+1;
            }
            _objsFile << "\nend\n\n";
        }
    }
    _numVertex += maxLines*maxCols;
}

#endif // FILEHANDLERS_HPP
