
#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>

using namespace std;

int main (  )
{
    string s;
    list < string > vote;
    vector < list < string > > votes;
    map < string, size_t > candidates;

    while ( true )
    {
        getline ( cin, s );
        if ( cin.eof (  ) )
        {
            if ( !vote.empty (  ) ) votes.push_back ( vote );
            break;
        }

        if ( s == "vote:" )
        {
            if ( !vote.empty (  ) )
            {
                votes.push_back ( vote );
                vote.clear (  );
            }
        }
        else
        {
            candidates.insert ( map < string, size_t >::value_type ( s, 0 ) );
            vote.push_back ( s );
        }
    }

    bool winner = false;
    size_t min_votes;
    map < string, size_t >::iterator c_iter, c_next;
    vector < list < string > >::iterator vl_iter;

    while ( !winner )
    {
        c_iter = candidates.begin (  );
        while ( c_iter != candidates.end (  ) )
        {
            cout << "clearing votes for " << c_iter->first << endl;
            c_iter->second = 0;
            ++c_iter;
        }

        vl_iter = votes.begin (  );
        while ( vl_iter != votes.end (  ) )
        {
            list < string >::iterator v_iter ( vl_iter->begin (  ) );
            while ( v_iter != vl_iter->end (  ) )
            {
                c_iter = candidates.find ( *v_iter );
                if ( c_iter != candidates.end (  ) )
                {
                    ++( c_iter->second );
                    break;
                }

                ++v_iter;
            }

            ++vl_iter;
        }

        c_iter = candidates.begin (  );
        min_votes = c_iter->second;
        winner = true;
        while ( c_iter != candidates.end (  ) )
        {
            cout << c_iter->first << " has a total of " << c_iter->second << " votes" << endl;
            if ( c_iter->second != min_votes )
            {
                winner = false;
            }
            if ( c_iter->second < min_votes )
            {
                min_votes = c_iter->second;
            }
            ++c_iter;
        }

        if ( winner )
        {
            if ( candidates.size (  ) > 1 )
            {
                cout << endl << "TIE, WINNERS WERE:" << endl;
            }
            else
            {
                cout << endl << "WINNER:" << endl;
            }
        }

        c_iter = candidates.begin (  );
        while ( c_iter != candidates.end (  ) )
        {
            c_next = c_iter;
            ++c_next;

            if ( winner )
            {
                cout << c_iter->first << endl;
            }
            else if ( c_iter->second == min_votes )
            {
                cout << c_iter->first << " has been removed from the running" << endl;
                candidates.erase ( c_iter );
            }

            c_iter = c_next;
        }
    }

    return 0;
}
