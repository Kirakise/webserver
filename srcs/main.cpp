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

int main()
{
    ServerBlock sv;
    sv.file = "configs/ex.conf";
    sv.ParseAll();
    sv.servers[0]._autoindex = true;
    sv.servers[1]._autoindex = true;
    FixBlock(sv);
    Cluster cl(sv.servers);
    cl.setup();
    cl.run();
    cl.clear();
}