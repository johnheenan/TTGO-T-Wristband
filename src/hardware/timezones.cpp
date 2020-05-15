#include "timezones.hpp"

int dst_index;
int dst_length;

// enum week_t {Last, First, Second, Third, Fourth};
// enum dow_t {Sun, Mon, Tue, Wed, Thu, Fri, Sat};
// enum month_t {Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};

// doco
// doco If you do not want some entries to appear then put them after the NULL entry line
// doco Also only up to the first 100 entries will be allowed to show. Can change this by changing DST_MAX
dst_t dst_array[] = { //doco there must be at least one try in adition to NULL
    {"Europe, Central", Last, Sun, "CEST", Mar, 2, 120, "CET", Oct, 3, 60}, //doco 
    {"China", Last, Sun, "", Mar, 2, 480, "", Oct, 3, 480}, //doco    // no DST
    {"Brisbane", First, Sun, "AEST", Apr, 3, 600, "AEST", Oct, 2, 600},    // doco no DST QLD as represented by Brisbane
    {"Sydney", First, Sun, "AEDT", Apr, 3, 600, "AEST", Oct, 2, 660},      // doco
    {"NULL"},  // doco please do not delete this NULL line, it is vital. Entries after this line will not be visible
};  //doco
// doco
