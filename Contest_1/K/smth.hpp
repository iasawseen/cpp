//
// Created by ivan on 28.03.17.
//

#ifndef K_SMTH_HPP
#define K_SMTH_HPP

#endif //K_SMTH_HPP


void func()
{
    {
        for (int day = 1; day < days_num + 1; day++)
        {
            auto it = pq.rbegin();
            auto first_pair = *it;
            it++;
            string leading_city = first_pair.second;
            if (it == pq.rend() || first_pair.first > it->first) {
                cities_leaderships[leading_city]++;
            }

            while (transition_it != transitions.end() && transition_it->day == day) {
                if (transition_it->day < day) {
                    cout « "wtf" « endl;
                }
                Oligarch& person = oligarchs[transition_it->name];
                string old_location = person.location;
                string new_location = transition_it->location;

                if (cities_net_worths.find(new_location) == cities_net_worths.end()) {
                    cities_net_worths[new_location] = 0;
                    cities_leaderships[new_location] = 0;
                    pq.insert(make_pair(0, new_location));
                }

                person.location = new_location;

                pq.erase(make_pair(cities_net_worths[old_location], old_location));
                cities_net_worths[old_location] -= person.wealth;
                pq.insert(make_pair(cities_net_worths[old_location], old_location));

                pq.erase(make_pair(cities_net_worths[new_location], new_location));
                cities_net_worths[new_location] += person.wealth;
                pq.insert(make_pair(cities_net_worths[new_location], new_location));
                transition_it++;
            }
        }