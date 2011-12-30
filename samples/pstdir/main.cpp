#include <iostream>        // wcout

#include "fairport/pst.h"

using namespace fairport;
using namespace std;
using namespace std::tr1::placeholders;

void process_message(int tab_depth, const message& m)
{
    for(int i = 0; i < tab_depth; ++i) cout << '\t';

    try {
        wcout << m.get_subject() << endl;
    }
    catch(key_not_found<prop_id>&)
    {
        wcout << L"<no subject>\n";
    }

}

void process_folder(int tab_depth, const folder& f)
{
    for(int i = 0; i < tab_depth; ++i) cout << '\t';
    wcout << f.get_name() << L" (" << f.get_message_count() << L")\n";

    for(folder::message_iterator m = f.message_begin(); m != f.message_end(); ++m)
    {
        process_message(tab_depth+1, *m);
    }

    for(folder::folder_iterator subf = f.sub_folder_begin(); subf != f.sub_folder_end(); ++subf)
    {
        process_folder(tab_depth+1, *subf);
    }

}

int main(int, char** argv)
{
    string path(argv[1]);
    wstring wpath(path.begin(), path.end());
    pst store(wpath);

    process_folder(0, store.open_root_folder());
}
