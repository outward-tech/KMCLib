/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


// Include the test definition.
#include "test_process.h"

// Include the files to test.
#include "process.h"

#include "configuration.h"
#include "random.h"

#include <cmath>

// -------------------------------------------------------------------------- //
//
void Test_Process::testConstruction()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::string> elements1;
    elements1.push_back("A");
    elements1.push_back("B");

    std::vector<std::string> elements2;
    elements2.push_back("C");
    elements2.push_back("B");

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    std::vector<int> basis_sites(3,0);
    basis_sites[1] = 23;
    basis_sites[2] = 11;
    const double rate = 13.7;
    Process process(config1, config2, rate, basis_sites);

    // Check that there are no listed indices by default.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // Check that the basis sites can be returned correctly.
    const std::vector<int> ret_basis_sites = process.basisSites();
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[0], basis_sites[0] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[1], basis_sites[1] );
    CPPUNIT_ASSERT_EQUAL( ret_basis_sites[2], basis_sites[2] );

}


// -------------------------------------------------------------------------- //
//
void Test_Process::testMatchList()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 123;
    possible_types["B"] = 24;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::string> elements1;
    elements1.push_back("A");
    elements1.push_back("B");
    elements1.push_back("C");

    std::vector<std::string> elements2;
    elements2.push_back("C");
    elements2.push_back("B");
    elements2.push_back("A");

    // Setup coordinates.
    std::vector<std::vector<double> > coords(3,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;
    coords[2][0] =  0.1;
    coords[2][1] =  0.3;
    coords[2][2] = -0.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const std::vector<int> basis_sites(1,0);
    const double rate = 13.7;
    Process process(config1, config2, rate, basis_sites);

    // Get the match list out.
    const std::vector<MinimalMatchListEntry> & match_list = process.minimalMatchList();

    // Check the size of the match list.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(match_list.size()), 3);

    // Get the first entry out and check.
    {
        const MinimalMatchListEntry entry = match_list[0];
        CPPUNIT_ASSERT_EQUAL(entry.index, -1);
        CPPUNIT_ASSERT_EQUAL(entry.match_type, 123);
        CPPUNIT_ASSERT_EQUAL(entry.update_type, 0);

        // Make sure these coordinates are equal.
        Coordinate check_coord(coords[0][0],coords[0][1],coords[0][2]);
        CPPUNIT_ASSERT( !(entry.coordinate < check_coord) );
        CPPUNIT_ASSERT( !(check_coord        < entry.coordinate) );

        // Check the distance.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(entry.distance, 0.0, 1.0e-14);

    }

    // Get the third entry out and check.
    {
        const MinimalMatchListEntry entry = match_list[2];
        CPPUNIT_ASSERT_EQUAL(entry.index, -1);
        CPPUNIT_ASSERT_EQUAL(entry.match_type, 24);
        CPPUNIT_ASSERT_EQUAL(entry.update_type, 24);

        // Make sure these coordinates are equal.
        Coordinate check_coord(coords[1][0],coords[1][1],coords[1][2]);
        CPPUNIT_ASSERT( !(entry.coordinate < check_coord) );
        CPPUNIT_ASSERT( !(check_coord      < entry.coordinate) );

        // Check the distance.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(entry.distance, check_coord.distance(Coordinate(0.0,0.0,0.0)), 1.0e-14);

    }

    // Get the second entry out and check.
    {
        const MinimalMatchListEntry entry = match_list[1];
        CPPUNIT_ASSERT_EQUAL(entry.index, -1);
        CPPUNIT_ASSERT_EQUAL(entry.match_type, 0);
        CPPUNIT_ASSERT_EQUAL(entry.update_type, 123);

        // Make sure these coordinates are equal.
        Coordinate check_coord(coords[2][0],coords[2][1],coords[2][2]);
        CPPUNIT_ASSERT( !(entry.coordinate < check_coord) );
        CPPUNIT_ASSERT( !(check_coord      < entry.coordinate) );

        // Check the distance.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(entry.distance, check_coord.distance(Coordinate(0.0,0.0,0.0)), 1.0e-14);

    }
}


// -------------------------------------------------------------------------- //
//
void Test_Process::testMatchListLong()
{
    // Setup the mapping from element to integer.
    std::map<std::string, int> possible_types;
    possible_types["A"] = 0;
    possible_types["B"] = 1;
    possible_types["C"] = 2;
    possible_types["D"] = 3;
    possible_types["E"] = 4;
    possible_types["F"] = 5;

    // Setup the two configurations.
    std::vector<std::string> elements1;
    elements1.push_back("A");
    elements1.push_back("B");
    elements1.push_back("B");
    elements1.push_back("B");
    elements1.push_back("B");
    elements1.push_back("B");
    elements1.push_back("B");
    elements1.push_back("B");
    elements1.push_back("B");
    std::vector<std::string> elements2;
    elements2.push_back("C");
    elements2.push_back("B");
    elements2.push_back("B");
    elements2.push_back("B");
    elements2.push_back("B");
    elements2.push_back("B");
    elements2.push_back("B");
    elements2.push_back("B");
    elements2.push_back("B");

    // Setup coordinates.
    std::vector<std::vector<double> > process_coords(9,std::vector<double>(3,0.0));
    process_coords[1][0] =  0.5;
    process_coords[1][1] =  0.5;
    process_coords[1][2] =  0.5;

    process_coords[2][0] = -0.5;
    process_coords[2][1] =  0.5;
    process_coords[2][2] =  0.5;

    process_coords[3][0] =  0.5;
    process_coords[3][1] = -0.5;
    process_coords[3][2] =  0.5;

    process_coords[4][0] =  0.5;
    process_coords[4][1] =  0.5;
    process_coords[4][2] = -0.5;

    process_coords[5][0] = -0.5;
    process_coords[5][1] = -0.5;
    process_coords[5][2] =  0.5;

    process_coords[6][0] = -0.5;
    process_coords[6][1] =  0.5;
    process_coords[6][2] = -0.5;

    process_coords[7][0] =  0.5;
    process_coords[7][1] = -0.5;
    process_coords[7][2] = -0.5;

    process_coords[8][0] = -0.5;
    process_coords[8][1] = -0.5;
    process_coords[8][2] = -0.5;

    // The configurations.
    const Configuration config1(process_coords, elements1, possible_types);
    const Configuration config2(process_coords, elements2, possible_types);


    // Check the length of the coordinates from the configuration.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(config1.coordinates().size()),
                          static_cast<int>(process_coords.size()));

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    Process process(config1, config2, rate, basis_sites);

    // Get the match list out.
    const std::vector<MinimalMatchListEntry> match_list = process.minimalMatchList();

    // Check the size of the match list.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(process_coords.size()),
                          static_cast<int>(match_list.size()));

    // Check the coordinates.
    const Coordinate c0 = match_list[0].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c0.x(), 0.0, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c0.y(), 0.0, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c0.z(), 0.0, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[0].distance, 0.0, 1.0e-14 );

    const Coordinate c1 = match_list[1].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.x(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.y(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.z(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[1].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c2 = match_list[2].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c2.x(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c2.y(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c2.z(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[2].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c3 = match_list[3].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c3.x(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c3.y(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c3.z(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[3].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c4 = match_list[4].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c4.x(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c4.y(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c4.z(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[4].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c5 = match_list[5].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c5.x(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c5.y(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c5.z(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[5].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c6 = match_list[6].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c6.x(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c6.y(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c6.z(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[6].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c7 = match_list[7].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c7.x(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c7.y(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c7.z(),-0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[7].distance, std::sqrt(0.25*3.0), 1.0e-14 );

    const Coordinate c8 = match_list[8].coordinate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c8.x(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c8.y(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( c8.z(), 0.5, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( match_list[8].distance, std::sqrt(0.25*3.0), 1.0e-14 );

}


// -------------------------------------------------------------------------- //
//
void Test_Process::testAddAndRemoveSite()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::string> elements1;
    elements1.push_back("A");
    elements1.push_back("B");

    std::vector<std::string> elements2;
    elements2.push_back("C");
    elements2.push_back("B");

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    Process process(config1, config2, rate, basis_sites);

    // Check that there are no listed indices by default.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // Add a few indices.
    process.addSite(1234);
    process.addSite(3);
    process.addSite(11);
    process.addSite(-123);

    // Check that these indices are now there.
    CPPUNIT_ASSERT( process.isListed(1234) );
    CPPUNIT_ASSERT( process.isListed(3)    );
    CPPUNIT_ASSERT( process.isListed(11)   );
    CPPUNIT_ASSERT( process.isListed(-123) );
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 4);

    // Remove two of the indices.
    process.removeSite(3);
    process.removeSite(11);

    // Check again.
    CPPUNIT_ASSERT( process.isListed(1234) );
    CPPUNIT_ASSERT( !process.isListed(3)   );
    CPPUNIT_ASSERT( !process.isListed(11)  );
    CPPUNIT_ASSERT( process.isListed(-123) );
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 2);

    // Remove the remaining two indices.
    process.removeSite(-123);
    process.removeSite(1234);

    // Check.
    CPPUNIT_ASSERT( !process.isListed(1234) );
    CPPUNIT_ASSERT( !process.isListed(3)    );
    CPPUNIT_ASSERT( !process.isListed(11)   );
    CPPUNIT_ASSERT( !process.isListed(-123) );
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(process.nSites()), 0);

    // This will crash since it is illegal.
    // process.removeSite(-123);
    // process.removeSite(1234);

    // DONE
}


// -------------------------------------------------------------------------- //
//
void Test_Process::testPickSite()
{
    // Default construct a process.
    Process process;

    // Add sites.
    process.addSite(12);
    process.addSite(199);
    process.addSite(19);

    // Get the cite.
    int counter12  = 0;
    int counter19  = 0;
    int counter199 = 0;

    seedRandom(false, 97);
    const int n_loop = 1000000;

    for (int i = 0; i < n_loop; ++i)
    {
        const int site = process.pickSite();
        CPPUNIT_ASSERT( ! (site != 12 && site != 199 && site != 19) );

        // Count how often each gets selected.
        if (site == 12)
        {
            ++counter12;
        }

        if (site == 199)
        {
            ++counter199;
        }

        if (site == 19)
        {
            ++counter19;
        }
    }

    // Test.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0 * n_loop / (3 * n_loop) , 1.0 * counter12 / n_loop,  1.0e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0 * n_loop / (3 * n_loop) , 1.0 * counter19 / n_loop,  1.0e-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0 * n_loop / (3 * n_loop) , 1.0 * counter199 / n_loop, 1.0e-2);

}


// -------------------------------------------------------------------------- //
//
void Test_Process::testAffectedIndices()
{
    // Setup a valid possible types map.
    std::map<std::string,int> possible_types;
    possible_types["A"] = 1;
    possible_types["B"] = 2;
    possible_types["C"] = 0;

    // Setup the two configurations.
    std::vector<std::string> elements1;
    elements1.push_back("A");
    elements1.push_back("B");

    std::vector<std::string> elements2;
    elements2.push_back("C");
    elements2.push_back("B");

    // Setup coordinates.
    std::vector<std::vector<double> > coords(2,std::vector<double>(3,0.0));
    coords[1][0] =  1.0;
    coords[1][1] =  1.3;
    coords[1][2] = -4.4;

    // The configurations.
    const Configuration config1(coords, elements1, possible_types);
    const Configuration config2(coords, elements2, possible_types);

    // Construct the process.
    const double rate = 13.7;
    const std::vector<int> basis_sites(1,0);
    Process process(config1, config2, rate, basis_sites);

    // Check that the size of the affected is correct.
    size_t one = 1;
    CPPUNIT_ASSERT_EQUAL( process.affectedIndices().size(), one );

    // Make sure we can add at some position.
    process.affectedIndices()[1] = 123;

    // And access.
    CPPUNIT_ASSERT_EQUAL( process.affectedIndices()[1], 123 );

}
