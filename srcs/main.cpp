#include "../includes/Ultimate.hpp"
size_t line_number;
void FixLock(ServerConf &sc, Location *l = 0, std::string curroot = "")
{
    if (curroot == "")
        curroot = sc._root;
    if (l == 0)
    {
        for (int i = 0; i < sc.locs.size(); i++)
        {
            if (sc.locs[i]._root == "")
                sc.locs[i]._root = curroot + sc.locs[i].locations[0];
            std::cout << sc.locs[i]._root << std::endl;
            FixLock(sc, &sc.locs[i], sc.locs[i]._root);
        }
    }
    else
    {
        for (int i = 0; i < l->locs.size(); i++)
        {
            if (l->locs[i]._root == "")
                l->locs[i]._root = curroot + l->locs[i].locations[0];
            std::cout << l->locs[i]._root << std::endl;
            FixLock(sc, &l->locs[i], l->locs[i]._root);
        }
    }
}

void FixBlock(ServerBlock &sv)
{
        for (int i = 0; i < sv.servers.size(); i++)
            FixLock(sv.servers[i]);
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
    ServerBlock sv;
    sv.file = argv[1];
    sv.ParseAll();
    if (sv.badConfig){
        std::cerr << "Fix config pls, i'm not working with this jewish thing" << std::endl;
        return (0);
    }
    
    FixBlock(sv);
    Cluster cl(sv.servers);
    if (cl.setup() != -1)
        cl.run();
    cl.clear();
    }
    else
        std::cout << "No conf file";
    return (0);
}