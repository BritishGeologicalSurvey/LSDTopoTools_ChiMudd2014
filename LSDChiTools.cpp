//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDChiTools
// Land Surface Dynamics ChiTools object
//
// An object within the University
//  of Edinburgh Land Surface Dynamics group topographic toolbox
//  for performing various analyses in chi space
//
//
// Developed by:
//  Simon M. Mudd
//  Martin D. Hurst
//  David T. Milodowski
//  Stuart W.D. Grieve
//  Declan A. Valters
//  Fiona Clubb
//
// Copyright (C) 2016 Simon M. Mudd 2016
//
// Developer can be contacted by simon.m.mudd _at_ ed.ac.uk
//
//    Simon Mudd
//    University of Edinburgh
//    School of GeoSciences
//    Drummond Street
//    Edinburgh, EH8 9XP
//    Scotland
//    United Kingdom
//
// This program is free software;
// you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation;
// either version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY;
// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the
// GNU General Public License along with this program;
// if not, write to:
// Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301
// USA
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// LSDChiTools.cpp
// LSDChiTools object
// LSD stands for Land Surface Dynamics
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This object is written by
// Simon M. Mudd, University of Edinburgh
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//-----------------------------------------------------------------
//DOCUMENTATION URL: http://www.geos.ed.ac.uk/~s0675405/LSD_Docs/
//-----------------------------------------------------------------



#ifndef LSDChiTools_CPP
#define LSDChiTools_CPP

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "TNT/tnt.h"
#include "LSDFlowInfo.hpp"
#include "LSDRaster.hpp"
#include "LSDIndexRaster.hpp"
#include "LSDChannel.hpp"
#include "LSDJunctionNetwork.hpp"
#include "LSDIndexChannel.hpp"
#include "LSDStatsTools.hpp"
#include "LSDShapeTools.hpp"
#include "LSDChiTools.hpp"
#include "LSDBasin.hpp"
#include "LSDChiNetwork.hpp"
using namespace std;
using namespace TNT;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Creates an LSDChiTools from an LSDRaster
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::create(LSDRaster& ThisRaster)
{
  NRows = ThisRaster.get_NRows();
  NCols = ThisRaster.get_NCols();
  XMinimum = ThisRaster.get_XMinimum();
  YMinimum = ThisRaster.get_YMinimum();
  DataResolution = ThisRaster.get_DataResolution();
  NoDataValue = ThisRaster.get_NoDataValue();
  GeoReferencingStrings = ThisRaster.get_GeoReferencingStrings();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Creates an LSDChiTools from an LSDRaster
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::create(LSDIndexRaster& ThisRaster)
{
  NRows = ThisRaster.get_NRows();
  NCols = ThisRaster.get_NCols();
  XMinimum = ThisRaster.get_XMinimum();
  YMinimum = ThisRaster.get_YMinimum();
  DataResolution = ThisRaster.get_DataResolution();
  NoDataValue = ThisRaster.get_NoDataValue();
  GeoReferencingStrings = ThisRaster.get_GeoReferencingStrings();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Creates an LSDChiTools from an LSDFlowInfo
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::create(LSDFlowInfo& ThisFI)
{
  NRows = ThisFI.get_NRows();
  NCols = ThisFI.get_NCols();
  XMinimum = ThisFI.get_XMinimum();
  YMinimum = ThisFI.get_YMinimum();
  DataResolution = ThisFI.get_DataResolution();
  NoDataValue = ThisFI.get_NoDataValue();
  GeoReferencingStrings = ThisFI.get_GeoReferencingStrings();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Creates an LSDChiTools from an LSDFlowInfo
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::create(LSDJunctionNetwork& ThisJN)
{
  NRows = ThisJN.get_NRows();
  NCols = ThisJN.get_NCols();
  XMinimum = ThisJN.get_XMinimum();
  YMinimum = ThisJN.get_YMinimum();
  DataResolution = ThisJN.get_DataResolution();
  NoDataValue = ThisJN.get_NoDataValue();
  GeoReferencingStrings = ThisJN.get_GeoReferencingStrings();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This resets all the data maps
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::reset_data_maps()
{
  map<int,float> empty_map;
  vector<int> empty_vec;

  M_chi_data_map = empty_map;
  b_chi_data_map = empty_map;
  elev_data_map = empty_map;
  chi_data_map = empty_map;
  flow_distance_data_map = empty_map;
  drainage_area_data_map = empty_map;
  node_sequence = empty_vec;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This function returns the x and y location of a row and column
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::get_x_and_y_locations(int row, int col, double& x_loc, double& y_loc)
{

  x_loc = XMinimum + float(col)*DataResolution + 0.5*DataResolution;

  // Slightly different logic for y because the DEM starts from the top corner
  y_loc = YMinimum + float(NRows-row)*DataResolution - 0.5*DataResolution;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This function returns the x and y location of a row and column
// Same as above but with floats
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::get_x_and_y_locations(int row, int col, float& x_loc, float& y_loc)
{

  x_loc = XMinimum + float(col)*DataResolution + 0.5*DataResolution;

  // Slightly different logic for y because the DEM starts from the top corner
  y_loc = YMinimum + float(NRows-row)*DataResolution - 0.5*DataResolution;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Function to convert a node position with a row and column to a lat
// and long coordinate
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::get_lat_and_long_locations(int row, int col, double& lat,
                   double& longitude, LSDCoordinateConverterLLandUTM Converter)
{
  // get the x and y locations of the node
  double x_loc,y_loc;
  get_x_and_y_locations(row, col, x_loc, y_loc);

  // get the UTM zone of the node
  int UTM_zone;
  bool is_North;
  get_UTM_information(UTM_zone, is_North);
  //cout << endl << endl << "Line 1034, UTM zone is: " << UTM_zone << endl;


  if(UTM_zone == NoDataValue)
  {
    lat = NoDataValue;
    longitude = NoDataValue;
  }
  else
  {
    // set the default ellipsoid to WGS84
    int eId = 22;

    double xld = double(x_loc);
    double yld = double(y_loc);

    // use the converter to convert to lat and long
    double Lat,Long;
    Converter.UTMtoLL(eId, yld, xld, UTM_zone, is_North, Lat, Long);


    lat = Lat;
    longitude = Long;
  }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This function gets the UTM zone
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::get_UTM_information(int& UTM_zone, bool& is_North)
{

  // set up strings and iterators
  map<string,string>::iterator iter;

  //check to see if there is already a map info string
  string mi_key = "ENVI_map_info";
  iter = GeoReferencingStrings.find(mi_key);
  if (iter != GeoReferencingStrings.end() )
  {
    string info_str = GeoReferencingStrings[mi_key] ;

    // now parse the string
    vector<string> mapinfo_strings;
    istringstream iss(info_str);
    while( iss.good() )
    {
      string substr;
      getline( iss, substr, ',' );
      mapinfo_strings.push_back( substr );
    }
    UTM_zone = atoi(mapinfo_strings[7].c_str());
    //cout << "Line 1041, UTM zone: " << UTM_zone << endl;
    //cout << "LINE 1042 LSDRaster, N or S: " << mapinfo_strings[7] << endl;

    // find if the zone is in the north
    string n_str = "n";
    string N_str = "N";
    is_North = false;
    size_t found = mapinfo_strings[8].find(N_str);
    if (found!=std::string::npos)
    {
      is_North = true;
    }
    found = mapinfo_strings[8].find(n_str);
    if (found!=std::string::npos)
    {
      is_North = true;
    }
    //cout << "is_North is: " << is_North << endl;

  }
  else
  {
    UTM_zone = NoDataValue;
    is_North = false;
  }

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This updates the chi values using a new chi raster
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::update_chi_data_map(LSDFlowInfo& FlowInfo, LSDRaster& Chi_coord)
{
  if (chi_data_map.size() == 0)
  {
    cout << "Trying to update chi but you have not run the automator yet to" << endl;
    cout << "organise the sources and channels. LSDChiTools::update_chi_data_map" << endl;
  }
  else
  {
    int n_nodes = int(node_sequence.size());
    int this_node,row,col;
    float updated_chi;
    for(int node = 0; node<n_nodes; node++)
    {
      this_node = node_sequence[node];
      FlowInfo.retrieve_current_row_and_col(this_node,row,col);
      updated_chi = Chi_coord.get_data_element(row,col);
      chi_data_map[this_node] = updated_chi;
    }
  }

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This updates the chi values by calculating them directly from the FlowInfo object
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::update_chi_data_map(LSDFlowInfo& FlowInfo, float A_0, float movern)
{
  if (chi_data_map.size() == 0)
  {
    cout << "Trying to update chi but you have not run the automator yet to" << endl;
    cout << "organise the sources and channels. LSDChiTools::update_chi_data_map" << endl;
  }
  else
  {
    float thresh_area_for_chi = 0;  // this gets chi in all nodes. Not much slower and avoids errors
    LSDRaster this_chi_coordinate = FlowInfo.get_upslope_chi_from_all_baselevel_nodes(movern,A_0,thresh_area_for_chi);

    int n_nodes = int(node_sequence.size());
    int this_node,row,col;
    float updated_chi;
    for(int node = 0; node<n_nodes; node++)
    {
      this_node = node_sequence[node];
      FlowInfo.retrieve_current_row_and_col(this_node,row,col);
      updated_chi = this_chi_coordinate.get_data_element(row,col);
      chi_data_map[this_node] = updated_chi;
    }
  }

}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This prints a chi map to csv with an area threshold in m^2
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::chi_map_to_csv(LSDFlowInfo& FlowInfo, string chi_map_fname,
                                 float A_0, float m_over_n, float area_threshold)
{

  ofstream chi_map_csv_out;
  chi_map_csv_out.open(chi_map_fname.c_str());

  chi_map_csv_out.precision(9);

  float chi_coord;
  double latitude,longitude;
  LSDCoordinateConverterLLandUTM Converter;

  chi_map_csv_out << "latitude,longitude,chi" << endl;

  LSDRaster Chi = FlowInfo.get_upslope_chi_from_all_baselevel_nodes(m_over_n, A_0, area_threshold);

  float NDV = Chi.get_NoDataValue();

  for(int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      chi_coord =  Chi.get_data_element(row,col);

      if (chi_coord != NDV)
      {
        get_lat_and_long_locations(row, col, latitude, longitude, Converter);
        chi_map_csv_out << latitude << "," << longitude  << "," << chi_coord << endl;
      }
    }
  }

  chi_map_csv_out.close();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This prints a chi map to csv with an area threshold in m^2. You feed it the chi map
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::chi_map_to_csv(LSDFlowInfo& FlowInfo, string chi_map_fname,
                                 LSDRaster& chi_coord)
{

  ofstream chi_map_csv_out;
  chi_map_csv_out.open(chi_map_fname.c_str());



  float this_chi_coord;
  double latitude,longitude;
  LSDCoordinateConverterLLandUTM Converter;

  chi_map_csv_out << "latitude,longitude,chi" << endl;

  float NDV = chi_coord.get_NoDataValue();

  for(int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      this_chi_coord = chi_coord.get_data_element(row,col);

      if (this_chi_coord != NDV)
      {
        get_lat_and_long_locations(row, col, latitude, longitude, Converter);
        chi_map_csv_out.precision(9);
        chi_map_csv_out << latitude << "," << longitude  << ",";
        chi_map_csv_out.precision(5);
        chi_map_csv_out << this_chi_coord << endl;
      }
    }
  }

  chi_map_csv_out.close();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This prints a chi map to csv with an area threshold in m^2. You feed it the chi map
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::chi_map_to_csv(LSDFlowInfo& FlowInfo, string chi_map_fname,
                                 LSDRaster& chi_coord, LSDIndexRaster& basin_raster)
{

  ofstream chi_map_csv_out;
  chi_map_csv_out.open(chi_map_fname.c_str());



  float this_chi_coord;
  int this_basin_number;
  double latitude,longitude;
  LSDCoordinateConverterLLandUTM Converter;

  chi_map_csv_out << "latitude,longitude,chi,basin_junction" << endl;

  float NDV = chi_coord.get_NoDataValue();

  for(int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      this_chi_coord = chi_coord.get_data_element(row,col);
      this_basin_number = basin_raster.get_data_element(row,col);


      if (this_chi_coord != NDV)
      {
        get_lat_and_long_locations(row, col, latitude, longitude, Converter);
        chi_map_csv_out.precision(9);
        chi_map_csv_out << latitude << "," << longitude  << ",";
        chi_map_csv_out.precision(5);
        chi_map_csv_out << this_chi_coord << ",";
        chi_map_csv_out << this_basin_number << endl;
      }
    }
  }

  chi_map_csv_out.close();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function creates the data structures for keeping track of
//  the channel network but only maps the chi coordinate.
// Mainly used for calculating m/n ratio.
// DOES NOT segment the chi-elevation profiles
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::chi_map_automator_chi_only(LSDFlowInfo& FlowInfo,
                                    vector<int> source_nodes,
                                    vector<int> outlet_nodes,
                                    vector<int> baselevel_node_of_each_basin,
                                    LSDRaster& Elevation, LSDRaster& FlowDistance,
                                    LSDRaster& DrainageArea, LSDRaster& chi_coordinate)
{
  // These elements access the chi data
  vector< vector<float> > chi_coordinates;
  vector< vector<int> > chi_node_indices;

  // these are for the individual channels
  vector<float> these_chi_coordinates;
  vector<int> these_chi_node_indices;

  // these are maps that will store the data
  map<int,float> chi_coord_map;
  map<int,float> elev_map;
  map<int,float> area_map;
  map<int,float> flow_distance_map;
  vector<int> node_sequence_vec;

  // these are vectors that will store information about the individual nodes
  // that allow us to map the nodes to specific channels during data visualisation

  // These two maps have each node in the channel (the index)
  // linked to a key (either the baselevel key or source key)
  map<int,int> these_source_keys;
  map<int,int> these_baselevel_keys;

  // These two maps link keys, which are incrmented by one, to the
  // junction or node of the baselevel or source
  map<int,int> this_key_to_source_map;
  map<int,int> this_key_to_baselevel_map;

  // these are for working with the FlowInfo object
  int this_node,row,col;
  int this_base_level, this_source_node;

  vector<int> empty_vec;
  ordered_baselevel_nodes = empty_vec;
  ordered_source_nodes = empty_vec;


  // get the number of channels
  int source_node_tracker = -1;
  int baselevel_tracker = -1;
  int ranked_source_node_tracker = -1;
  int n_channels = int(source_nodes.size());
  for(int chan = 0; chan<n_channels; chan++)
  {
    //cout << "Sampling channel " << chan+1 << " of " << n_channels << endl;

    // get the base level
    this_base_level = baselevel_node_of_each_basin[chan];
    //cout << "Got the base level" << endl;

    // If a key to this base level does not exist, add one.
    if ( this_key_to_baselevel_map.find(this_base_level) == this_key_to_baselevel_map.end() )
    {
      baselevel_tracker++;

      // this resets the ranked source node tracker
      ranked_source_node_tracker = -1;

      //cout << "Found a new baselevel. The node is: " << this_base_level << " and key is: " << baselevel_tracker << endl;
      this_key_to_baselevel_map[this_base_level] = baselevel_tracker;
      ordered_baselevel_nodes.push_back(this_base_level);
    }

    // now add the source tracker
    source_node_tracker++;
    ranked_source_node_tracker++;

    // get the source node
    this_source_node = source_nodes[chan];

    // add the node to the trackers so that we can trace individual basin nodes
    // for m over n calculations
    ordered_source_nodes.push_back(this_source_node);
    source_nodes_ranked_by_basin.push_back(ranked_source_node_tracker);

    // now add the source node to the data map
    this_key_to_source_map[this_source_node] = source_node_tracker;

    //cout << "The source key is: " << source_node_tracker << " and basin key is: " << baselevel_tracker << endl;

    // get this particular channel (it is a chi network with only one channel)
    LSDChiNetwork ThisChiChannel(FlowInfo, source_nodes[chan], outlet_nodes[chan],
                                Elevation, FlowDistance, DrainageArea,chi_coordinate);

    // okay the ChiNetwork has all the data about the m vales at this stage.
    // Get these vales and print them to a raster
    chi_coordinates = ThisChiChannel.get_chis();
    chi_node_indices = ThisChiChannel.get_node_indices();

    // now get the number of channels. This should be 1!
    int n_channels = int(chi_coordinates.size());
    if (n_channels != 1)
    {
      cout << "Whoa there, I am trying to make a chi map but something seems to have gone wrong with the channel extraction."  << endl;
      cout << "I should only have one channel per look but I have " << n_channels << " channels." << endl;
    }

    // now get chi coordantes
    these_chi_coordinates = chi_coordinates[0];
    these_chi_node_indices = chi_node_indices[0];

    //cout << "I have " << these_chi_m_means.size() << " nodes." << endl;


    int n_nodes_in_channel = int(these_chi_coordinates.size());
    for (int node = 0; node< n_nodes_in_channel; node++)
    {

      this_node =  these_chi_node_indices[node];
      //cout << "This node is " << this_node << endl;

      // only take the nodes that have not been found
      if (chi_coord_map.find(this_node) == chi_coord_map.end() )
      {
        FlowInfo.retrieve_current_row_and_col(this_node,row,col);

        //cout << "This is a new node; " << this_node << endl;
        chi_coord_map[this_node] = these_chi_coordinates[node];
        elev_map[this_node] = Elevation.get_data_element(row,col);
        area_map[this_node] = DrainageArea.get_data_element(row,col);
        flow_distance_map[this_node] = FlowDistance.get_data_element(row,col);
        node_sequence_vec.push_back(this_node);

        these_source_keys[this_node] = source_node_tracker;
        these_baselevel_keys[this_node] = baselevel_tracker;
      }
      else
      {
        //cout << "I already have node: " << this_node << endl;
      }
    }
  }

  //cout << "I am all finished segmenting the channels!" << endl;

  // set the object data members
  elev_data_map = elev_map;
  chi_data_map = chi_coord_map;
  flow_distance_data_map = flow_distance_map;
  drainage_area_data_map = area_map;
  node_sequence = node_sequence_vec;

  source_keys_map = these_source_keys;
  baselevel_keys_map = these_baselevel_keys;
  key_to_source_map = this_key_to_source_map;
  key_to_baselevel_map = this_key_to_baselevel_map;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function is for calculating segments from all sources in a DEM
// The sources and their outlets are supplied by the source and outlet nodes
// vectors. These are generated from the LSDJunctionNetwork function
// get_overlapping_channels
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::chi_map_automator(LSDFlowInfo& FlowInfo,
                                    vector<int> source_nodes,
                                    vector<int> outlet_nodes,
                                    vector<int> baselevel_node_of_each_basin,
                                    LSDRaster& Elevation, LSDRaster& FlowDistance,
                                    LSDRaster& DrainageArea, LSDRaster& chi_coordinate,
                                    int target_nodes,
                                    int n_iterations, int skip,
                                    int minimum_segment_length, float sigma)
{

  // IMPORTANT THESE PARAMETERS ARE NOT USED BECAUSE CHI IS CALCULATED SEPARATELY
  // However we need to give something to pass to the Monte carlo functions
  // even through they are not used (they are inherited)
  float A_0 = 1;
  float m_over_n = 0.5;

  // These elements access the chi data
  vector< vector<float> > chi_m_means;
  vector< vector<float> > chi_b_means;
  vector< vector<float> > chi_coordinates;
  vector< vector<int> > chi_node_indices;

  // these are for the individual channels
  vector<float> these_chi_m_means;
  vector<float> these_chi_b_means;
  vector<float> these_chi_coordinates;
  vector<int> these_chi_node_indices;

  // these are maps that will store the data
  map<int,float> m_means_map;
  map<int,float> b_means_map;
  map<int,float> chi_coord_map;
  map<int,float> elev_map;
  map<int,float> area_map;
  map<int,float> flow_distance_map;
  vector<int> node_sequence_vec;

  // these are vectors that will store information about the individual nodes
  // that allow us to map the nodes to specific channels during data visualisation

  // These two maps have each node in the channel (the index)
  // linked to a key (either the baselevel key or source key)
  map<int,int> these_source_keys;
  map<int,int> these_baselevel_keys;

  // These two maps link keys, which are incrmented by one, to the
  // junction or node of the baselevel or source
  map<int,int> this_key_to_source_map;
  map<int,int> this_key_to_baselevel_map;

  // these are for working with the FlowInfo object
  int this_node,row,col;
  int this_base_level, this_source_node;

  // get the number of channels
  int source_node_tracker = -1;
  int baselevel_tracker = -1;
  int ranked_source_node_tracker = -1;
  int n_channels = int(source_nodes.size());
  for(int chan = 0; chan<n_channels; chan++)
  {
    //cout << "Sampling channel " << chan+1 << " of " << n_channels << endl;

    // get the base level
    this_base_level = baselevel_node_of_each_basin[chan];
    //cout << "Got the base level" << endl;

    // If a key to this base level does not exist, add one.
    if ( this_key_to_baselevel_map.find(this_base_level) == this_key_to_baselevel_map.end() )
    {
      baselevel_tracker++;
      // this resets the ranked source node tracker
      ranked_source_node_tracker = -1;
      //cout << "Found a new baselevel. The node is: " << this_base_level << " and key is: " << baselevel_tracker << endl;
      this_key_to_baselevel_map[this_base_level] = baselevel_tracker;
      ordered_baselevel_nodes.push_back(this_base_level);
    }

    // now add the source tracker
    source_node_tracker++;
    ranked_source_node_tracker++;

    // get the source node
    this_source_node = source_nodes[chan];

    // add the node to the trackers so that we can trace individual basin nodes
    // for m over n calculations
    ordered_source_nodes.push_back(this_source_node);
    source_nodes_ranked_by_basin.push_back(ranked_source_node_tracker);

    // now add the source node to the data map
    this_key_to_source_map[this_source_node] = source_node_tracker;

    //cout << "The source key is: " << source_node_tracker << " and basin key is: " << baselevel_tracker << endl;

    // get this particular channel (it is a chi network with only one channel)
    LSDChiNetwork ThisChiChannel(FlowInfo, source_nodes[chan], outlet_nodes[chan],
                                Elevation, FlowDistance, DrainageArea,chi_coordinate);

    // split the channel
    //cout << "Splitting channels" << endl;
    ThisChiChannel.split_all_channels(A_0, m_over_n, n_iterations, skip, target_nodes, minimum_segment_length, sigma);

    // monte carlo sample all channels
    //cout << "Entering the monte carlo sampling" << endl;
    ThisChiChannel.monte_carlo_sample_river_network_for_best_fit_after_breaks(A_0, m_over_n, n_iterations, skip, minimum_segment_length, sigma);

    // okay the ChiNetwork has all the data about the m vales at this stage.
    // Get these vales and print them to a raster
    chi_m_means = ThisChiChannel.get_m_means();
    chi_b_means = ThisChiChannel.get_b_means();
    chi_coordinates = ThisChiChannel.get_chis();
    chi_node_indices = ThisChiChannel.get_node_indices();

    // now get the number of channels. This should be 1!
    int n_channels = int(chi_m_means.size());
    if (n_channels != 1)
    {
      cout << "Whoa there, I am trying to make a chi map but something seems to have gone wrong with the channel extraction."  << endl;
      cout << "I should only have one channel per look but I have " << n_channels << " channels." << endl;
    }

    // now get the m_means out
    these_chi_m_means = chi_m_means[0];
    these_chi_b_means = chi_b_means[0];
    these_chi_coordinates = chi_coordinates[0];
    these_chi_node_indices = chi_node_indices[0];

    //cout << "I have " << these_chi_m_means.size() << " nodes." << endl;


    int n_nodes_in_channel = int(these_chi_m_means.size());
    for (int node = 0; node< n_nodes_in_channel; node++)
    {

      this_node =  these_chi_node_indices[node];
      //cout << "This node is " << this_node << endl;

      // only take the nodes that have not been found
      if (m_means_map.find(this_node) == m_means_map.end() )
      {
        FlowInfo.retrieve_current_row_and_col(this_node,row,col);

        //cout << "This is a new node; " << this_node << endl;
        m_means_map[this_node] = these_chi_m_means[node];
        b_means_map[this_node] = these_chi_b_means[node];
        chi_coord_map[this_node] = these_chi_coordinates[node];
        elev_map[this_node] = Elevation.get_data_element(row,col);
        area_map[this_node] = DrainageArea.get_data_element(row,col);
        flow_distance_map[this_node] = FlowDistance.get_data_element(row,col);
        node_sequence_vec.push_back(this_node);

        these_source_keys[this_node] = source_node_tracker;
        these_baselevel_keys[this_node] = baselevel_tracker;

      }
      else
      {
        //cout << "I already have node: " << this_node << endl;
      }
    }
  }

  //cout << "I am all finished segmenting the channels!" << endl;

  // set the object data members
  M_chi_data_map =m_means_map;
  b_chi_data_map = b_means_map;
  elev_data_map = elev_map;
  chi_data_map = chi_coord_map;
  flow_distance_data_map = flow_distance_map;
  drainage_area_data_map = area_map;
  node_sequence = node_sequence_vec;

  source_keys_map = these_source_keys;
  baselevel_keys_map = these_baselevel_keys;
  key_to_source_map = this_key_to_source_map;
  key_to_baselevel_map = this_key_to_baselevel_map;

  // get the fitted elevations
  calculate_segmented_elevation(FlowInfo);

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function is a much more rudimentary version that mimics the
// channel steepness caluclations.
// chi needs to be calculated outside of the function
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::chi_map_automator_rudimentary(LSDFlowInfo& FlowInfo,
                                    vector<int> source_nodes,
                                    vector<int> outlet_nodes,
                                    vector<int> baselevel_node_of_each_basin,
                                    LSDRaster& Elevation, LSDRaster& FlowDistance,
                                    LSDRaster& DrainageArea, LSDRaster& chi_coordinate,
                                    int regression_nodes)
{

  // the data is stored in maps, for easier testing if a node has been
  // visited.
  // You might consider having these as data elements in the object so you don't
  // have to pass them
  map<int,float> gradient_data_map;
  map<int,float> intercept_data_map;
  map<int,float> R2_data_map;
  map<int,float> chi_coordinate_data_map;
  map<int,float> elevation_data_map;
  map<int,float> flow_distance_map;
  map<int,float> area_map;
  vector<int> node_order;

  // check if the number of nodes are odd .If not add 1
  if (regression_nodes % 2 == 0)
  {
    cout << "Hello user. You need an odd number of regression nodes." << endl;
    regression_nodes = regression_nodes+1;
    cout << " Changing your regression nodes to " << regression_nodes << endl;
  }

  // now get the midpoint
  int mp_nodes = (regression_nodes-1)/2;

  //cout << "The number of mp nodes is: " << mp_nodes << endl;

  // these keep track of the beginning and ending nodes of a given channel
  int channel_start_node;
  int channel_end_node;
  float channel_end_elevation;

  // vectors for holding the chi elevation data
  vector<float> chi_vec;
  vector<float> elev_vec;
  vector<float> empty_vec;

  // these are extracted from the channel segments using linear regression
  float intercept,gradient,R_squared;

  //float this_chi;
  //float this_elev;
  int this_mp_node;
  int this_end_node;
  int this_start_node;

  // these are for getting information out of the FlowInfo object
  int row,col, this_node;
  int r_node, r_row,r_col;          // reciever row and column.

  // The way this works is that it starts at the top of a channel. It then works
  // its way down and find the node that is the midpoint and the node that is the
  // end point. The midpoint node is where the data will be recorded.
  // It then puts the data from the start node to the end node into a vector
  // and performs a linear regression of this vector. The regression data from these
  // vectors are recorded at the nodes.
  // We then want to cover all the nodes with data so what happens if some nodes
  // do not become midpoints?
  // We could start at the top and get the first midpoint.
  // From there we can work our way down checking if the top of the regression segment
  // is more than one node down from the end point...

  // get the number of channels
  int n_channels = int(source_nodes.size());
  // now loop through the channels
  for(int chan = 0; chan<n_channels; chan++)
  {
    channel_start_node = source_nodes[chan];
    channel_end_node = outlet_nodes[chan];

    // Get the elevation of the end node as a secondary check of the ending of the channel
    // segment
    FlowInfo.retrieve_current_row_and_col(channel_end_node,row,col);
    channel_end_elevation = Elevation.get_data_element(row,col);

    // reset the flag for ending the channel
    bool is_end_of_channel = false;

    // set the segment start node to the channel start node
    this_start_node = channel_start_node;

    // now retrieve the midpoint node
    this_node = channel_start_node;
    for(int n = 0; n<mp_nodes; n++)
    {
      FlowInfo.retrieve_receiver_information(this_node,r_node,r_row,r_col);
      this_node = r_node;
    }
    this_mp_node = this_node;
    this_node = r_node;

    // now go down one step
    FlowInfo.retrieve_receiver_information(this_node,r_node,r_row,r_col);
    this_node = r_node;

    // now get the end node
    for(int n = 0; n<mp_nodes; n++)
    {
      FlowInfo.retrieve_receiver_information(this_node,r_node,r_row,r_col);
      this_node = r_node;
    }
    this_end_node = this_node;

    //================================================
    // This loop is for bug checking
    //this_node = this_start_node;
    //do
    //{
    //  // get the elevation and chi vectors by following the flow
    //  cout << "This node is: " << this_node << endl;
    //  FlowInfo.retrieve_current_row_and_col(this_node,row,col);
    //  FlowInfo.retrieve_receiver_information(this_node,r_node,r_row,r_col);
    //  this_node = r_node;
    //}
    //while(this_node != this_end_node);
    //
    //cout << "And the midpoint node was: " << this_mp_node << endl;
    //================================================

    // we search down the channel, collecting linear regressions at the
    // midpoint of the intervals
    while (not is_end_of_channel)
    {
      // get a vector of chi and elevation from the start node to the end node
      chi_vec = empty_vec;
      elev_vec = empty_vec;

      // copy the data elements into the vecotrs. This is a little stupid
      // because one might just use a deque to pop the first element
      // and push the last, but the linear regression takes vectors,
      // not deques so you would have to copy the deque element-wise anyway
      // If you wanted, you could speed this up by implementing a linear regression
      // of deques, but that will need to wait for another day.
      this_node = this_start_node;
      do
      {
        // get the elevation and chi vectors by following the flow
        FlowInfo.retrieve_current_row_and_col(this_node,row,col);
        chi_vec.push_back(chi_coordinate.get_data_element(row,col));
        elev_vec.push_back(Elevation.get_data_element(row,col));

        FlowInfo.retrieve_receiver_information(this_node,r_node,r_row,r_col);
        this_node = r_node;
      } while(this_node != this_end_node);

      // do a linear regression on the segment
      least_squares_linear_regression(chi_vec,elev_vec, intercept, gradient, R_squared);

      // now add the intercept and gradient data to the correct node
      // only take data that has not been calculated before
      // The channels are in order of descending length so data from
      // longer channels take precidence.
      if (gradient_data_map.find(this_mp_node) == gradient_data_map.end() )
      {
        FlowInfo.retrieve_current_row_and_col(this_mp_node,row,col);
        gradient_data_map[this_mp_node] = gradient;
        intercept_data_map[this_mp_node] = intercept;
        R2_data_map[this_mp_node] = R_squared;
        chi_coordinate_data_map[this_mp_node] = chi_coordinate.get_data_element(row,col);
        elevation_data_map[this_mp_node] = Elevation.get_data_element(row,col);
        flow_distance_map[this_mp_node] = FlowDistance.get_data_element(row,col);
        area_map[this_mp_node] = DrainageArea.get_data_element(row,col);
        node_order.push_back(this_mp_node);
      }
      else
      {
        is_end_of_channel = true;
      }

      // now move all the nodes down one
      FlowInfo.retrieve_receiver_information(this_start_node,r_node,r_row,r_col);
      this_start_node = r_node;

      FlowInfo.retrieve_receiver_information(this_mp_node,r_node,r_row,r_col);
      this_mp_node = r_node;

      FlowInfo.retrieve_receiver_information(this_end_node,r_node,r_row,r_col);
      this_end_node = r_node;

      // check if we are at the end of the channel
      if (this_end_node == channel_end_node)
      {
        is_end_of_channel = true;
      }
      // also check if the end node is lower elevation than the end node,
      // just to try and stop the channel passing the end node
      FlowInfo.retrieve_current_row_and_col(this_end_node,row,col);
      if (channel_end_elevation > Elevation.get_data_element(row,col))
      {
        is_end_of_channel = true;
      }
    }          // This finishes the regression segment loop
  }            // This finishes the channel and resets channel start and end nodes

  // set the data objects
  M_chi_data_map = gradient_data_map;
  b_chi_data_map = intercept_data_map;
  elev_data_map = elevation_data_map;
  chi_data_map = chi_coordinate_data_map;
  flow_distance_data_map = flow_distance_map;
  drainage_area_data_map = area_map;
  node_sequence = node_order;


}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function calculates the basins and an additional file that has basin centroids
// and labelling information for plotting
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDIndexRaster LSDChiTools::get_basin_raster(LSDFlowInfo& FlowInfo, LSDJunctionNetwork& JunctionNetwork,
                               vector<int> Junctions)
{
  int N_Juncs = Junctions.size();
  LSDCoordinateConverterLLandUTM Converter;


  // Get some data members for holding basins and the raster
  vector<LSDBasin> AllTheBasins;
  map<int,int> drainage_of_other_basins;
  LSDIndexRaster BasinMasterRaster;

  //cout << "I am trying to print basins, found " << N_BaseLevelJuncs << " base levels." << endl;
  // Loop through the junctions
  for(int BN = 0; BN<N_Juncs; BN++)
  {
    //cout << "Getting basin " << BN << " and the junction is: "  << BaseLevelJunctions[BN] << endl;
    LSDBasin thisBasin(Junctions[BN],FlowInfo, JunctionNetwork);
    //cout << "...got it!" << endl;
    AllTheBasins.push_back(thisBasin);

    // This is required if the basins are nested--test the code which numbers
    // to be overwritten by a smaller basin
    drainage_of_other_basins[Junctions[BN]] = thisBasin.get_NumberOfCells();

  }

  // now loop through everything again getting the raster
  if (N_Juncs > 0)     // this gets the first raster
  {
    BasinMasterRaster = AllTheBasins[0].write_integer_data_to_LSDIndexRaster(Junctions[0], FlowInfo);
  }

  // now add on the subsequent basins
  for(int BN = 1; BN<N_Juncs; BN++)
  {
    AllTheBasins[BN].add_basin_to_LSDIndexRaster(BasinMasterRaster, FlowInfo,
                              drainage_of_other_basins, Junctions[BN]);
  }

  return BasinMasterRaster;

}



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function is used to tag channels with a segment number
// It decides on segments if the M_Chi value has changed so should only be used
// with chi networks that have used a skip of 0 and a monte carlo itertions of 1
// This data is used by other routines to look at the spatial distribution of
// hillslope-channel coupling.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::segment_counter(LSDFlowInfo& FlowInfo)
{
  // these are for extracting element-wise data from the channel profiles.
  int this_node;
  int segment_counter = 0;
  map<int,int> this_segment_counter_map;
  float last_M_chi, this_M_chi;

  // find the number of nodes
  int n_nodes = (node_sequence.size());
  if (n_nodes <= 0)
  {
    cout << "Cannot calculate segments since you have not calculated channel properties yet." << endl;
  }
  else
  {
    this_node = node_sequence[0];
    last_M_chi =  M_chi_data_map[this_node];

    for (int n = 0; n< n_nodes; n++)
    {

      // Get the M_chi from the current node
      this_node = node_sequence[n];
      this_M_chi = M_chi_data_map[this_node];

      // If the M_chi has changed, increment the segment counter
      if (this_M_chi != last_M_chi)
      {
        segment_counter++;
        last_M_chi = this_M_chi;
      }

      // Print the segment counter to the data map
      this_segment_counter_map[this_node]  = segment_counter;
    }
  }
  segment_counter_map = this_segment_counter_map;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function is used to tag channels with a segment number
// It decides on segments if the M_Chi value has changed so should only be used
// with chi networks that have used a skip of 0 and a monte carlo itertions of 1
// This data is used by other routines to look at the spatial distribution of
// hillslope-channel coupling. Generates an LSDIndexRaster of the channel network
// indexed by segment numbers for feeding to the hilltop flow routing analysis.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
LSDIndexRaster LSDChiTools::segment_mapping(LSDFlowInfo& FlowInfo)
{
  // these are for extracting element-wise data from the channel profiles.
  int this_node, row, col;
  int segment_counter = 0;
  map<int,int> this_segment_counter_map;
  float last_M_chi, this_M_chi;

  //declare empty array for raster generation
  Array2D<int> SegmentedStreamNetworkArray(NRows,NCols,-9999);

  // find the number of nodes
  int n_nodes = (node_sequence.size());
  if (n_nodes <= 0)
  {
    cout << "Cannot calculate segments since you have not calculated channel properties yet." << endl;
  }
  else
  {
    //get the node
    this_node = node_sequence[0];
    FlowInfo.retrieve_current_row_and_col(this_node,row,col);

    last_M_chi =  M_chi_data_map[this_node];

    for (int n = 0; n< n_nodes; n++)
    {

      // Get the M_chi from the current node
      this_node = node_sequence[n];
      this_M_chi = M_chi_data_map[this_node];

      // If the M_chi has changed, increment the segment counter
      if (this_M_chi != last_M_chi)
      {
        segment_counter++;
        last_M_chi = this_M_chi;
      }

      // Print the segment counter to the data map and raster
      this_segment_counter_map[this_node]  = segment_counter;
      SegmentedStreamNetworkArray[row][col] = segment_counter;
    }
  }
  segment_counter_map = this_segment_counter_map;

  return LSDIndexRaster(NRows,NCols,XMinimum,YMinimum,DataResolution,NoDataValue,SegmentedStreamNetworkArray,GeoReferencingStrings);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function is used to tag channels with a segment number
// It decides on segments if the M_Chi value has changed so should only be used
// with chi networks that have used a skip of 0 and a monte carlo itertions of 1
// This data is used by other routines to look at the spatial distribution of
// hillslope-channel coupling.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::segment_counter_knickpoint(LSDFlowInfo& FlowInfo, float threshold_knickpoint, float threshold_knickpoint_length)
{
  // these are for extracting element-wise data from the channel profiles.
  //int abs_threshhold_knickpoint = abs (threshold_knickpoint);
  int this_node = 0;
  int segment_counter_knickpoint = 0; // count the number of knickpoints
  int segment_counter = 0; // count the number of segments
  map<int,float> this_segment_counter_knickpoint_map;
  map<int,int> this_segment_counter_map;
  map<int,int> this_segment_knickpoint_sign_map;
  map<int,int> this_segment_length_map;
  float last_M_chi, this_M_chi;
  float delta_m = 0; // difference between last and new m_chi
  int knickpoint_sign = 0; // sign of the knickpoint: + =1 and - = -1
  float temp_delta_m = 0; // debugging stuff
  float this_segment_length = 0;
  int last_node = 0;
  int n_nodes_segment = 0;
  float x1_temp =0;
  float y1_temp =0;
  float x2_temp =0;
  float y2_temp =0;
  bool same_channel;
  float max_knickpoint_value =0;
  int new_knickpoint_counter =0;


  // find the number of nodes
  int n_nodes = (node_sequence.size());
  if (n_nodes <= 0)
  {
    cout << "Cannot calculate segments since you have not calculated channel properties yet." << endl;
  }
  else
  {
    this_node = node_sequence[0];
    last_M_chi =  M_chi_data_map[this_node];

    for (int n = 0; n< n_nodes; n++)
    {

      // set the nodes number and keep information about the previous one
      if(n>0)
      {
        last_node = this_node;
      }
      this_node = node_sequence[n];
      // Get the M_chi from the current node
      this_M_chi = M_chi_data_map[this_node];
      // increment the segment node counter
      n_nodes_segment++;

      // If the M_chi has changed, do stuffs
      if (this_M_chi != last_M_chi)
      {
        segment_counter++; // increment the  segment counter
        delta_m= last_M_chi/this_M_chi; // Ratio between last and new chi steepness
        if(delta_m<1){knickpoint_sign = -1;} else {knickpoint_sign = 1;} // Assign the knickpoint sign value
        //delta_m = abs(delta_m); // required to detect all the knickpoints
        //if(delta_m > temp_delta_m) {temp_delta_m = delta_m;} // debugging stuff
        // now checking if the difference of m_chi between two segment is not due to a channel change
          // first retrieving xy coordinates
        FlowInfo.get_x_and_y_from_current_node(last_node, x1_temp, y1_temp);
        FlowInfo.get_x_and_y_from_current_node(this_node, x2_temp, y2_temp);

        // Then check if the distance betweenthe two is more than 2 nodes (distance between two points via pytagore or thing like this)
        if (sqrt((x2_temp - x1_temp)*(x2_temp - x1_temp)+(y2_temp - y1_temp)*(y2_temp - y1_temp)) > (2*FlowInfo.get_DataResolution()))
        {
          same_channel = false;
        }
        // done



        // Check if the threshold is (over)reached
        //if(delta_m > abs_threshhold_knickpoint && same_channel) if we are are using the threshold value

        // SMM NOTE: WHAT IS GOING ON HERE?
        // Is this supposed to be if(same_channel) ?

        if(true) // useless thing
        {
          segment_counter_knickpoint++; // number of knickpoints
          this_segment_counter_knickpoint_map[this_node] = delta_m; // adding the knickpoint value
          if(delta_m>max_knickpoint_value){max_knickpoint_value=delta_m; cout << max_knickpoint_value <<"||"<<this_segment_counter_knickpoint_map[this_node] << endl; } // assign the new knickpoint max value
        }
        //this_segment_length = n_nodes_segment * FlowInfo.get_DataResolution(); // getting the length of the segment using the resolution * number of nodes
        same_channel = true; // Set back the same channel parameter to true
        // now assign the segment lenght to all the point of the segment
        for(int i = n_nodes_segment ; i > 0 ; i--)
        {
          this_segment_length_map[node_sequence[n - i]] =  this_segment_length;
        }

        last_M_chi = this_M_chi;
        this_segment_knickpoint_sign_map[this_node] = knickpoint_sign; // assign the segment polarity
        n_nodes_segment = 0; // reinitialyse the # of node for the next segment
        this_segment_length = 0; // same
      }
      else
      {
        // incrementing the segment length
        if (n>0)
        {
          // Now calculating the distance between the two last nodes

          // Retrieving the x/y coordinates of the last two nodes
          FlowInfo.get_x_and_y_from_current_node(last_node, x1_temp, y1_temp);
          FlowInfo.get_x_and_y_from_current_node(this_node, x2_temp, y2_temp);
          // calculate and increment the distance from the last node
          this_segment_length += sqrt((x2_temp - x1_temp)*(x2_temp - x1_temp)+(y2_temp - y1_temp)*(y2_temp - y1_temp));
          //cout << "distance : " << this_segment_length;

        }
      }


      // Print the segment counter to the data map
      this_segment_counter_map[this_node]  = segment_counter;
    }





    // Just Ignore this process for now!
    if(threshold_knickpoint_length>0)
    {


      // calculating equivalent between nodes_of_knickpoints and nodes to be able to properly navigate inside the map
      int nodes_of_knickpoints [segment_counter_knickpoint];
      int knickpoint_id = 0;
      int last_knickpoint_id = 0;
      for(int i = 0; i< n_nodes; i++)
      {
        this_node = node_sequence[i];
        if(this_segment_counter_knickpoint_map.count(this_node)) // supposed to test if this map has a value assigned for this key
        {
          nodes_of_knickpoints[knickpoint_id] = this_node;
          last_knickpoint_id = knickpoint_id; // stock the last knickpoint_id
          knickpoint_id++;
        }
      }
      // setting up the calculation for the length threshold
      bool still_processing = true;
      bool still_processing_total = true;
      float old_max_knickpoint_value = max_knickpoint_value;
      float distance_to_process_down = threshold_knickpoint_length/2; // represents the threshold down the knickpoint
      float distance_to_process_up = threshold_knickpoint_length/2; // represents the threshold up the knickpoint
      int number_of_erase = 0;

      while (still_processing_total)
      {
        still_processing = true;
        vector <int> knickpoint_to_delete;
        knickpoint_id = 0;

        map<int, float>::iterator it = this_segment_counter_knickpoint_map.begin();
        while(still_processing && it != this_segment_counter_knickpoint_map.end())
        {
          //cout<<it->first<<" :: "<<it->second<<endl;
          if(it->second == max_knickpoint_value)
          {
            distance_to_process_down = threshold_knickpoint_length/2;
            distance_to_process_up = threshold_knickpoint_length/2;
            // let's know test the distance before and beyond this nodes
            for(int g = 1; distance_to_process_down>0 || distance_to_process_up >0 ;g++)
            {

              //Calculate the case down
              if(this_segment_counter_knickpoint_map.count(nodes_of_knickpoints[knickpoint_id - g]) && distance_to_process_down>0)
              {
                //cout << "down exists" << endl;
                this_node = nodes_of_knickpoints[knickpoint_id - g];
                last_node = nodes_of_knickpoints[knickpoint_id - g+1];
                FlowInfo.get_x_and_y_from_current_node(last_node, x1_temp, y1_temp);
                FlowInfo.get_x_and_y_from_current_node(this_node, x2_temp, y2_temp);
                float temp_distance = sqrt((x2_temp - x1_temp)*(x2_temp - x1_temp)+(y2_temp - y1_temp)*(y2_temp - y1_temp));
                if(temp_distance < distance_to_process_down)
                {
                  distance_to_process_down -= temp_distance;
                  knickpoint_to_delete.push_back(this_node);
                } else{distance_to_process_down = 0;}
              }
              else{distance_to_process_down = 0;}
              // calculate the case up
              if(this_segment_counter_knickpoint_map.count(nodes_of_knickpoints[knickpoint_id + g]) && distance_to_process_up> 0)
              {
                //cout << "up exists" << endl;
                this_node = nodes_of_knickpoints[knickpoint_id + g];
                last_node = nodes_of_knickpoints[knickpoint_id + g-1];
                FlowInfo.get_x_and_y_from_current_node(last_node, x1_temp, y1_temp);
                FlowInfo.get_x_and_y_from_current_node(this_node, x2_temp, y2_temp);
                float temp_distance = sqrt((x2_temp - x1_temp)*(x2_temp - x1_temp)+(y2_temp - y1_temp)*(y2_temp - y1_temp));
                if(temp_distance < distance_to_process_down)
                {
                  distance_to_process_down-=temp_distance;
                  knickpoint_to_delete.push_back(this_node);
                } else{distance_to_process_down = 0;}
              }
              else {distance_to_process_up = 0;}
            }
            // Now I have to erase everything I planned to
            for (std::vector<int>::iterator it2 = knickpoint_to_delete.begin() ; it2 != knickpoint_to_delete.end(); it2++)
            {
              this_segment_counter_knickpoint_map.erase(*it2);
              //cout << *it2 <<endl;
              number_of_erase++;
            }

            it = this_segment_counter_knickpoint_map.begin();
            knickpoint_to_delete.clear();

            // calculate the new maximum
            old_max_knickpoint_value = max_knickpoint_value;
            max_knickpoint_value = 0;
            for(int j = 0; j< segment_counter_knickpoint; j++)
            {
              //cout << "Am I reaching this point?? j: "<< j << endl;
              this_node = nodes_of_knickpoints[j];
              if(this_segment_counter_knickpoint_map.count(this_node))
              {
                if(this_segment_counter_knickpoint_map[this_node] > max_knickpoint_value && this_segment_counter_knickpoint_map[this_node] < old_max_knickpoint_value && j!=knickpoint_id )
                  {
                    max_knickpoint_value = this_segment_counter_knickpoint_map[this_node];

                  }
              }
            }
            // reset the internal loop
            still_processing = false;


          }
          else
          {

            it++;
            knickpoint_id++;
            cout <<"application of the length threshold: " << it->first << "||" << number_of_erase <<"||"<<old_max_knickpoint_value<<"||" <<max_knickpoint_value <<endl;
            if(it->first == nodes_of_knickpoints[last_knickpoint_id] || max_knickpoint_value == 0)
            {
              still_processing_total = false;
            }
          }
        }
      }
    }
    // alright, trying a new stuff for the length threshold this time











    /*


    if(false)
    {
      // now sorting and calculating the knickpoint values by length (hopefully)
      int nodes_of_knickpoints [segment_counter_knickpoint];
      map<int,vector <float> > temp_knickpoint_map;
      //map <int,float,float>
      int knickpoint_id = 0;
      for(int i = 0; i< n_nodes; i++)
      {
        this_node = node_sequence[i];
        if(this_segment_counter_knickpoint_map.count(this_node)) // supposed to test if this map has a value assigned for this key
        {
          nodes_of_knickpoints[knickpoint_id] = this_node;
          FlowInfo.get_x_and_y_from_current_node(this_node, x1_temp, y1_temp);
          float this_node_float = this_node;
          vector<float> temp_vecta (3);
          temp_vecta.push_back(this_node);
          temp_vecta.push_back(x1_temp);
          temp_vecta.push_back(y1_temp);
          temp_knickpoint_map[knickpoint_id] = temp_vecta;
          knickpoint_id++;
          cout << temp_vecta[1] << endl;

          //cout << this_node << " || " << nodes_of_knickpoints[knickpoint_id] << endl;
        }
      }
    }




    // this part begin the calculation for the length threshold to erase too dense areas and set new variables
    float distance_to_process_down = threshold_knickpoint_length/2; // represents the threshold
    float distance_to_process_up = threshold_knickpoint_length/2; // represents the threshold
    float distance_to_substract = 0; // temp variable that remove the distance already processed from the threshold
    bool still_processing = true; // tell the for loop when to reloop fromn the beginning
    bool still_processing_total = false; // tell the main loop when to stop
    int old_max = 0; // used to stock the previous maximum data
    new_knickpoint_counter = segment_counter_knickpoint; // will return the number of knickpoints after deletion of the oldest
    //number_of_nodes_to_investigate_length = threshold_knickpoint_length;
    // beginning the calculation
    cout << "beginning the length calculation stuffs" << endl;




    // old method, will erase if I found another way
    while(still_processing_total) // This will be shutted down when the last node will have been processed
    {
      still_processing = true; // Required to trigger next loop
      for(int i = 0; i< segment_counter_knickpoint && still_processing; i++)
      {
        this_node = nodes_of_knickpoints[i]; // go through the knickpoints
        if(this_segment_counter_knickpoint_map.count(this_node)) // check if this node still exists
        {
          if(  this_segment_counter_knickpoint_map[this_node]==max_knickpoint_value) // Check if it is the current maximum value
          {
            for(int g = 1; distance_to_process_down > 0 || distance_to_process_up > 0 ; g++) // If so, test the adjacent nodes in order to delete the required ones
            {
              // now getting the coordinates of the wanted nodes
              cout<<"aue coute"<<endl;
              if(this_segment_length_map.count(this_node-g) && this_segment_length_map.count(this_node-g+1))
              {

                FlowInfo.get_x_and_y_from_current_node(this_node-g, x1_temp, y1_temp);
                FlowInfo.get_x_and_y_from_current_node(this_node-g+1, x2_temp, y2_temp);


                // Check if it exists and if it is on the same river
                if(this_segment_counter_knickpoint_map.count(this_node-g) && distance_to_process_down >0)
                {
                  if(sqrt((x2_temp - x1_temp)*(x2_temp - x1_temp)+(y2_temp - y1_temp)*(y2_temp - y1_temp))<= distance_to_process_down)
                  {
                    this_segment_counter_knickpoint_map.erase(this_node-g);
                    this_segment_knickpoint_sign_map.erase(this_node-g);
                    new_knickpoint_counter--;
                    cout << "something to test blablabla" << endl;
                    distance_to_process_down -= sqrt((x2_temp - x1_temp)*(x2_temp - x1_temp)+(y2_temp - y1_temp)*(y2_temp - y1_temp));
                  }
                  else
                  {
                    distance_to_process_down = 0;
                  }
                }
              }
              else
              {
                distance_to_process_down = 0;
              }
              if(this_segment_length_map.count(this_node+g) && this_segment_length_map.count(this_node+g-1))
              {
                FlowInfo.get_x_and_y_from_current_node(this_node+g, x1_temp, y1_temp);
                FlowInfo.get_x_and_y_from_current_node(this_node+g-1, x2_temp, y2_temp);
                if(this_segment_counter_knickpoint_map.count(this_node+g) && distance_to_process_up>0)
                {
                  if(sqrt((x2_temp - x1_temp)*(x2_temp - x1_temp)+(y2_temp - y1_temp)*(y2_temp - y1_temp))<= distance_to_process_up)
                  {
                    cout << "before erase :" << this_segment_knickpoint_sign_map[this_node+g] << endl;
                    this_segment_counter_knickpoint_map.erase(this_node+g);
                    this_segment_knickpoint_sign_map.erase(this_node+g);
                    new_knickpoint_counter--;
                    cout << "after erase :" << this_segment_knickpoint_sign_map[this_node+g] << endl;
                    distance_to_process_up -= sqrt((x2_temp - x1_temp)*(x2_temp - x1_temp)+(y2_temp - y1_temp)*(y2_temp - y1_temp));
                  }
                  else
                  {
                    distance_to_process_up=0;
                  }
                }
              }
              else
              {
                distance_to_process_up=0;
              }
            }

            old_max = max_knickpoint_value;
            max_knickpoint_value = 0;
            for(int j = 0; j< segment_counter_knickpoint; j++)
            {
              //cout << "Am I reaching this point?? j: "<< j << endl;
              this_node = nodes_of_knickpoints[j];
              if(this_segment_counter_knickpoint_map.count(this_node))
              {
                if(this_segment_counter_knickpoint_map[this_node] > max_knickpoint_value && this_segment_counter_knickpoint_map[this_node] <= old_max)
                  {
                    max_knickpoint_value =this_segment_counter_knickpoint_map[this_node];
                    //if(this_segment_counter_knickpoint_map[this_node] == max_knickpoint_value) {cout<< "biiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiite"<< endl;}
                  }
              }
            }
            still_processing = false;
            distance_to_process_up = threshold_knickpoint_length/2;
            distance_to_process_down = threshold_knickpoint_length/2;
          }
        }
        if(i == segment_counter_knickpoint-1)
        {
          still_processing = false;
          still_processing_total = false;
        }
        else{still_processing = true;}
      }
    }*/
  }


  cout << "segment_counter_knickpoint is   " << new_knickpoint_counter << "/" << segment_counter << " delta max is " << temp_delta_m << endl;
  // print everything in the public/protected variables
  segment_counter_knickpoint_map = this_segment_counter_knickpoint_map;
  segment_knickpoint_sign_map = this_segment_knickpoint_sign_map;
  segment_length_map = this_segment_length_map;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// New function for the knickpoint detection
// save the difference and ratio between m_chi values of each segments
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::ksn_knickpoint_detection(LSDFlowInfo& FlowInfo)
{
  // these are for extracting element-wise data from the channel profiles.
  //int abs_threshhold_knickpoint = abs (threshold_knickpoint);
  int this_node = 0;
  map<int,float> this_kickpoint_diff_map;
  map<int,float> this_kickpoint_ratio_map;
  map<int,int> this_knickpoint_sign_map;
  float last_M_chi, this_M_chi;
  float delta_mchi = 0; // difference between last and new m_chi
  float ratio_mchi = 0; // ratio between last and new m_chi
  int knickpoint_sign = 0; // sign of the knickpoint: + =1 and - = -1
  int last_node = 0;
  int number_of_0 = 0;
  int n_knp = 0;




  // find the number of nodes
  int n_nodes = (node_sequence.size());
  if (n_nodes <= 0)
  {
    cout << "Cannot calculate segments since you have not calculated channel properties yet." << endl;
  }
  else
  {
    this_node = node_sequence[0];
    last_M_chi =  M_chi_data_map[this_node];

    for (int n = 0; n< n_nodes; n++)
    {

      // set the nodes number and keep information about the previous one
      if(n>0)
      {
        last_node = this_node;
      }
      this_node = node_sequence[n];
      // Get the M_chi from the current node
      this_M_chi = M_chi_data_map[this_node];

      if(this_M_chi < 0 && n>0){this_M_chi = 0;} // getting rid of the negative values because we don't want it, I don't want the n = 0 to avoid detecting fake knickpoint if the first value is actually negative

      // If the M_chi has changed I increment the knickpoints, I also check if the two point are on the same channel to avoid stange unrelated knickpoints
      if (this_M_chi != last_M_chi && key_to_source_map[this_node] == key_to_source_map[last_node])
      {
        if(this_M_chi == 0)
        {
          ratio_mchi = -9999; // correspond to +infinite
          number_of_0++;
        }
        else
        {
          ratio_mchi = last_M_chi/this_M_chi; // Ratio between last and new chi steepness
        }
        delta_mchi = last_M_chi-this_M_chi; // diff between last and new chi steepness
        if(delta_mchi<=0){knickpoint_sign = -1;} else {knickpoint_sign = 1;} // Assign the knickpoint sign value
        delta_mchi = abs(delta_mchi); // we want the absolute mangitude of this, the sign being displayed in another column. it is just like nicer like this.
        // Allocate the values to local maps
        this_kickpoint_diff_map[this_node] = delta_mchi;
        this_kickpoint_ratio_map[this_node] = ratio_mchi;
        this_knickpoint_sign_map[this_node] = knickpoint_sign;
        n_knp ++;

        // reinitialise the parameters for next loop turn
        last_M_chi = this_M_chi;
      }

    }

  }
  // print everything in the public/protected maps
  kns_ratio_knickpoint_map = this_kickpoint_ratio_map;
  kns_diff_knickpoint_map = this_kickpoint_diff_map;
  ksn_sign_knickpoint_map = this_knickpoint_sign_map;
  cout << "I finished to detect the knickpoints, you have " << n_knp << " knickpoints, thus " << number_of_0 << " ratios are switched to -9999 due to 0 divisions." << endl;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Print data maps to file - knickpoint version
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::print_knickpoint_to_csv(LSDFlowInfo& FlowInfo, string filename)
{

  // these are for extracting element-wise data from the channel profiles.
  cout << "I am now writing your ksn knickpoint file:" << endl;
  int this_node, row,col;
  double latitude,longitude;
  LSDCoordinateConverterLLandUTM Converter;

  // find the number of nodes
  int n_nodes = (node_sequence.size());

  // open the data file
  ofstream  chi_data_out;
  chi_data_out.open(filename.c_str());
  chi_data_out << "latitude,longitude,elevation,flow distance,drainage area,diff,ratio,sign,source_key,basin_key";

  chi_data_out << endl;




  if (n_nodes <= 0)
  {
    cout << "Cannot print since you have not calculated channel properties yet." << endl;
  }
  else
  {
    for (int n = 0; n< n_nodes; n++)
    {
        this_node = node_sequence[n];
        FlowInfo.retrieve_current_row_and_col(this_node,row,col);
        get_lat_and_long_locations(row, col, latitude, longitude, Converter);
        if(kns_diff_knickpoint_map.count(this_node) == 1)
        {
        chi_data_out.precision(9);
        chi_data_out << latitude << ","
                     << longitude << ",";
        chi_data_out.precision(5);
        chi_data_out << elev_data_map[this_node] << ","
                     << flow_distance_data_map[this_node] << ","
                     << drainage_area_data_map[this_node] << ","
                     << kns_diff_knickpoint_map[this_node] << ","
                     << kns_ratio_knickpoint_map[this_node] << ","
                     << ksn_sign_knickpoint_map[this_node] << ","
                     << source_keys_map[this_node] << ","
                     << baselevel_keys_map[this_node];

        chi_data_out << endl;
      }
    }
  }

  chi_data_out.close();
  cout << "I am done, your file is:" << endl;
  cout << filename << endl;

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function calculates the fitted elevations: It uses m_chi and b_chi
// data to get the fitted elevation of the channel points.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::calculate_segmented_elevation(LSDFlowInfo& FlowInfo)
{
  // these are for extracting element-wise data from the channel profiles.
  int this_node;
  map<int,float> this_segmented_elevation_map;
  float this_M_chi, this_b_chi, this_chi, this_segemented_elevation;

  // find the number of nodes
  int n_nodes = (node_sequence.size());
  if (n_nodes <= 0)
  {
    cout << "Cannot calculate segments since you have not calculated channel properties yet." << endl;
  }
  else
  {
    for (int n = 0; n< n_nodes; n++)
    {

      // Get the M_chi and b_chi from the current node
      this_node = node_sequence[n];
      this_M_chi = M_chi_data_map[this_node];
      this_b_chi = b_chi_data_map[this_node];
      this_chi = chi_data_map[this_node];

      // calculate elevations simply based on the fact that we are fitting segments
      // with the equation z = M_chi*chi+b_chi
      this_segemented_elevation = this_M_chi*this_chi+this_b_chi;

      // Print the segment counter to the data map
      this_segmented_elevation_map[this_node]  = this_segemented_elevation;
    }
  }
  segmented_elevation_map = this_segmented_elevation_map;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This renumbers sources by base level. Each base level node has
// a number of sources and these get an incremental value which is used
// for the combination vector. These serve as maps between the source keys
// and the MLE values
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::baselevel_and_source_splitter(vector<int>& n_sources_for_baselevel,
                                                vector<int>& index_into_sources_vec)
{

  // You need to loop through all the sources, for each baselevel you get all the number rankings
  int n_sources = int(ordered_source_nodes.size());
  int baselevel_node;

  // this vector contains the index into the ordered source node vector
  // of the starting point for a given baselelvel node
  vector<int> starting_index_of_source_for_baselevel_node;
  vector<int> n_sources_each_baselevel;

  // loop through allsources, tracking where the baselevel nodes change.
  int this_baselevel_node = -1;
  int n_sources_this_baselevel;
  for (int i = 0; i< n_sources; i++)
  {
    // get the baselevel node of each of the sources
    baselevel_node = baselevel_keys_map[ ordered_source_nodes[i] ];

    if(baselevel_node != this_baselevel_node)
    {
      if (this_baselevel_node != -1)
      {
        n_sources_each_baselevel.push_back(n_sources_this_baselevel);
      }


      starting_index_of_source_for_baselevel_node.push_back(i);
      this_baselevel_node = baselevel_node;

      n_sources_this_baselevel = 0;
    }

    n_sources_this_baselevel++;
  }

  // and now get the last number of baselelvel nodes
  n_sources_each_baselevel.push_back(n_sources_this_baselevel);

  // now print out the results
  //cout << endl << endl << "============" << endl;
  bool print_for_debug = false;
  if(print_for_debug)
  {
    for(int i = 0; i< n_sources; i++)
   {
      cout << "Source number is: " << ordered_source_nodes[i] << " and baselelvel: " << baselevel_keys_map[ ordered_source_nodes[i] ] << endl;
    }

    int n_bl = int(starting_index_of_source_for_baselevel_node.size());
    cout << endl << endl << "============" << endl;
    cout << "n_bl: " << n_bl << endl;
    for(int i = 0; i< n_bl; i++)
    {
      cout << "Baselevel node is: " << ordered_baselevel_nodes[i] << " n sources: "
          << n_sources_each_baselevel[i] << " start_index: "
          << starting_index_of_source_for_baselevel_node[i] << endl;
    }

    cout << endl << endl << "============" << endl;
    cout << "Let me get the numbereing for you by basin" << endl;
    for(int i = 0; i< n_sources; i++)
    {
      cout << "Renumbered source key is: " << source_nodes_ranked_by_basin[i] << endl;
    }

  }

  // replace the two vectors
  n_sources_for_baselevel = n_sources_each_baselevel,
  index_into_sources_vec = starting_index_of_source_for_baselevel_node;

}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function is used to debug the basin and source indexing
void LSDChiTools::print_basin_and_source_indexing_to_screen()
{
  cout << endl << endl << "=============" << endl << "printing for debugging" << endl;
  int n_sources = int(ordered_source_nodes.size());
  for(int i = 0; i< n_sources; i++)
  {
    cout << "Source number is: " << ordered_source_nodes[i] << " and baselelvel: " << baselevel_keys_map[ ordered_source_nodes[i] ] << endl;
  }

  vector<int> n_sources_for_baselevel;
  vector<int> index_into_sources_vec;
  baselevel_and_source_splitter(n_sources_for_baselevel,index_into_sources_vec);

  int n_bl = int(index_into_sources_vec.size());
  cout << endl << endl << "============" << endl;
  cout << "n_bl: " << n_bl << endl;
  for(int i = 0; i< n_bl; i++)
  {
    cout << "Baselevel node is: " << ordered_baselevel_nodes[i] << " n sources: "
        << n_sources_for_baselevel[i] << " start_index: "
        << index_into_sources_vec[i] << endl;
  }

  cout << endl << endl << "============" << endl;
  cout << "Let me get the numbering for you by basin" << endl;
  for(int i = 0; i< n_sources; i++)
  {
    cout << "Renumbered source key is: " << source_nodes_ranked_by_basin[i] << endl;
  }

  // now get the source keys
  cout << endl << endl << "========" << endl << "sources and keys" << endl;
  map<int,int>::iterator iter;
  iter = key_to_source_map.begin();
  while(iter != key_to_source_map.end())
  {
    cout << "source is: " << iter->first << " and key is: " << iter->second << endl;
    iter++;
  }

}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function test the collinearity of all segments compared to a reference
// segment
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
float LSDChiTools::test_segment_collinearity(LSDFlowInfo& FlowInfo, int reference_channel, int test_channel)
{
  // The way this works is that one of the segments (delineated by its source number)
  // is taken as a reference, and then all other segments are compared to how
  // closely they match this segment. If the chi value of the segment being tested
  // is greater than the maximum chi of the reference segment or less than the
  // minimum chi of the reference segment the data point is ignored.

  float MLE = 1;
  float sigma = 1000;
  // first get the source node of the reference channel
  if ( reference_channel >= int(key_to_source_map.size()) || test_channel >= int(key_to_source_map.size()) )
  {
    cout << "LSDChiTools::test_segment_collinearity One of the sources is not in the channel network. Source is: " << reference_channel << endl;
  }
  else
  {
    vector<float> elev_data_chan0;
    vector<float> chi_data_chan0;
    get_chi_elevation_data_of_channel(FlowInfo, reference_channel, chi_data_chan0, elev_data_chan0);

    vector<float> elev_data_chan1;
    vector<float> chi_data_chan1;
    get_chi_elevation_data_of_channel(FlowInfo, test_channel, chi_data_chan1, elev_data_chan1);

    vector<float> residuals = project_data_onto_reference_channel(chi_data_chan0, elev_data_chan0,
                                 chi_data_chan1,elev_data_chan1);
    MLE = calculate_MLE_from_residuals(residuals, sigma);

  }
  return MLE;

}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function test the collinearity of all segments compared to a reference
// segment
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
float LSDChiTools::test_all_segment_collinearity_by_basin(LSDFlowInfo& FlowInfo, bool only_use_mainstem_as_reference,
                                                 int baselevel_key,
                                                 vector<int>& reference_source, vector<int>& test_source,
                                                 vector<float>& MLE_values, vector<float>& RMSE_values)
{
  cout << "Testing the segment collinearity for basin key " << baselevel_key << endl;
  // get some information about the number of basins
  int n_basins = int(ordered_baselevel_nodes.size());
  if (baselevel_key >= n_basins)
  {
    cout << "Fatal error LSDChiTools::test_all_segment_collinearity_by_basin" <<endl;
    cout << "You have selected a basin that doesn't exist!" << endl;
    exit(EXIT_FAILURE);
  }


  // run the splitter
  // this gets the starting index of the sources for each basin.
  // It means that the channel numbers are linked to the channels in the basin
  vector<int> start_node_for_baselelvel;
  vector<int> n_sources_in_basin;
  baselevel_and_source_splitter(n_sources_in_basin, start_node_for_baselelvel);
  int channel_offset = start_node_for_baselelvel[baselevel_key];
  int n_channels = n_sources_in_basin[baselevel_key];

  // Drop out if there is only a single channel in the basin
  if (n_channels == 1)
  {
    cout << "This basin only has one channel." << endl;
    return 1.0;
  }


  //cout << "Let me check the basin indexing for you." << endl;
  //int n_bl =  int(start_node_for_baselelvel.size());
  //for(int i = 0; i< n_bl; i++)
  //{
  //  cout << "Baselevel node is: " << ordered_baselevel_nodes[i] << " n sources: "
  //      << n_sources_in_basin[i] << " start_index: "
  //      << start_node_for_baselelvel[i] << endl;
  //}
  //cout << "Basin key is: " << baselevel_key << " This basin has " << n_channels << " sources and start node is "<< channel_offset <<endl;

  // placeholder vectors: will replace the passed vectors
  vector<int> this_reference_source;
  vector<int> this_test_source;
  vector<float> these_MLE_values;
  vector<float> these_RMSE_values;

  // now get all the possible two pair combinations of these channels
  bool zero_indexed = true;   // this is just because the channels are numbered from zero
  int k = 2;                  // We want combinations of 2 channels

  // the vec vec holds a vector of each possible combination of channels
  // each vector has two elements in it: the first and second channel in the comibination
  vector< vector<int> > combo_vecvev = combinations(n_channels, k, zero_indexed);
  vector<float> elev_data_chan0;
  vector<float> chi_data_chan0;
  vector<float> elev_data_chan1;
  vector<float> chi_data_chan1;
  vector<float> residuals;

  int n_residuals;



  vector<float> MLEs;
  vector<int> MLE_index;   // the index into the combo_vecvec that is used to
                           // tell which combinations have MLE values

  float sigma = 1000;
  int last_ref_channel = -1;

  int n_combinations = int(combo_vecvev.size());
  vector<int> this_combo;
  int chan0,chan1;

  if (only_use_mainstem_as_reference)
  {
    n_combinations = n_channels-1;
  }

  // we loop through the different combinations in the vecvec
  for (int combo = 0; combo < n_combinations; combo++)
  {
    this_combo = combo_vecvev[combo];

    // you need to map these combinations onto the particular channels of this basin
    // These channels refere to the source keys
    chan0 = this_combo[0]+channel_offset;
    chan1 = this_combo[1]+channel_offset;
    //cout << "chan0 is: " << chan0 << "  and chan1 is: " << chan1 << " and combo 0 is: " << this_combo[0] <<endl;


    // only get the reference channel if the channel has changed.
    // This collects the chi-elevation data of the reference channel
    if (last_ref_channel != chan0)
    {
      get_chi_elevation_data_of_channel(FlowInfo, chan0, chi_data_chan0, elev_data_chan0);
    }

    // This gets the chi-elevation data of the test channel. Again, the chan1
    // parameter referes to the source key.
    get_chi_elevation_data_of_channel(FlowInfo, chan1, chi_data_chan1, elev_data_chan1);

    // Now return the residuals between the reference channel and test channel.
    // Each node in the test channel gets a residual, it is projected to a
    // linear fit between nodes on the reference channel
    residuals = project_data_onto_reference_channel(chi_data_chan0, elev_data_chan0,
                                 chi_data_chan1,elev_data_chan1);
    n_residuals = int(residuals.size());
    //cout << "Basin: " << baselevel_key << " The number of residuals are: " << n_residuals << endl;

    // Now get the MLE and RMSE for this channel pair. It only runs if
    // there are residuals. Otherwise it means that the channels are non-overlapping
    if (n_residuals > 0)
    {
      float MLE1 = calculate_MLE_from_residuals(residuals, sigma);
      float RMSE = calculate_RMSE_from_residuals(residuals);
      last_ref_channel = chan0;
      //cout << "MLE: " << MLE1 << " and RMSE: " << RMSE << endl;

      // If we are only using the mainstem channel, we only use the first channel
      // as a reference channel. The first channel is denoted by this_combo[0] == 0
      //cout << "The use only mainstem is: " << only_use_mainstem_as_reference << endl;
      if (only_use_mainstem_as_reference)
      {
        //cout << "Checking the combination, combo 0 is: " << this_combo[0] << endl;
        if (this_combo[0] > 0)
        {
          // skip to the last node
          //cout << "I am skipping to the LAST NODE in the combinations" << endl;
          combo = n_combinations;
        }
        else
        {
          these_MLE_values.push_back(MLE1);
          these_RMSE_values.push_back(RMSE);
          this_reference_source.push_back(chan0);
          this_test_source.push_back(chan1);
        }
      }
      else
      {
        these_MLE_values.push_back(MLE1);
        these_RMSE_values.push_back(RMSE);
        this_reference_source.push_back(chan0);
        this_test_source.push_back(chan1);
      }
    }
    else
    {
        these_MLE_values.push_back(1.0);
        these_RMSE_values.push_back(0.0);
        this_reference_source.push_back(chan0);
        this_test_source.push_back(chan1);
    }
  }

  MLE_values = these_MLE_values;
  RMSE_values = these_RMSE_values;
  reference_source = this_reference_source;
  test_source = this_test_source;

  float tot_MLE = 1;
  for (int res = 0; res < int(these_MLE_values.size()); res++)
  {
    tot_MLE = tot_MLE*these_MLE_values[res];
  }


  //cout << "Let me tell you all about the MLE values " << endl;
  // for debugging
  bool print_results = false;
  if(print_results)
  {
    for (int res = 0; res < int(MLE_values.size()); res++)
    {
      cout << "reference_source: " << reference_source[res] << " "
           << "test_source: " << test_source[res] << " "
           << "MLE_values: " << MLE_values[res] << " "
           << "RMSE_values: " << RMSE_values[res] << endl;
    }
  }

  //cout << "N_residuals: " << MLE_values.size() << endl << endl;

  //cout << "Total MLE is: " << tot_MLE << endl;
  return tot_MLE;

}



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function test the collinearity of all segments compared to a reference
// segment
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::calculate_goodness_of_fit_collinearity_fxn_movern(LSDFlowInfo& FlowInfo, LSDJunctionNetwork& JN,
                        float start_movern, float delta_movern, int n_movern,
                        bool only_use_mainstem_as_reference,
                        string file_prefix)
{
  // these vectors store all the values which are then used for printing
  vector< vector<float> > RMSE_vecvec;
  vector< vector<float> > MLE_vecvec;
  vector< vector<float> > total_MLE_vecvec;
  vector<int> reference_keys;
  vector<int> test_keys;
  vector<int> outlet_jns;

  cout << "LSDChiTools::calculate_goodness_of_fit_collinearity_fxn_movern" << endl;
  cout << "I am defaulting to A_0 = 1." << endl;
  vector<float> movern;
  float A_0 = 1;
  //float thresh_area_for_chi = 0;      // This just gets chi from all pixels.

  string filename_bstats = file_prefix+"_basinstats.csv";
  ofstream stats_by_basin_out;
  stats_by_basin_out.open(filename_bstats.c_str());

  int n_basins = int(ordered_baselevel_nodes.size());

  // get the outlet junction of each basin key
  for (int basin_key = 0; basin_key < n_basins; basin_key++)
  {
    int outlet_node = ordered_baselevel_nodes[basin_key];
    int outlet_jn = JN.get_Junction_of_Node(outlet_node, FlowInfo);
    outlet_jns.push_back(outlet_jn);
  }

  cout << endl << endl << "==========================" << endl;
  for(int i = 0; i< n_movern; i++)
  {
    // get the m over n value
    movern.push_back( float(i)*delta_movern+start_movern );
    cout << "i: " << i << " and m over n: " << movern[i] << " ";

    // open the outfile
    string filename_fullstats = file_prefix+"_"+dtoa(movern[i])+"_fullstats.csv";
    ofstream movern_stats_out;
    movern_stats_out.open(filename_fullstats.c_str());

    // calculate chi
    update_chi_data_map(FlowInfo, A_0, movern[i]);

    // these are the vectors that will hold the information about the
    // comparison between channels.
    // the _all vectors are one of all the basins
    // reference source is the source key of the reference channel
    vector<int> reference_source, all_reference_source;
    // test source is the source key of the test channel
    vector<int> test_source, all_test_source;
    // MLE the maximum liklihood estimator
    vector<float> MLE_values, all_MLE_values;
    // RMSE is the root mean square error
    vector<float> RMSE_values, all_RMSE_values;

    vector<float> tot_MLE_vec;
    // basin keys
    vector<int> all_basin_keys;

    // now run the collinearity test
    float tot_MLE;

    for(int basin_key = 0; basin_key<n_basins; basin_key++)
    {
      tot_MLE = test_all_segment_collinearity_by_basin(FlowInfo, only_use_mainstem_as_reference,
                                  basin_key,
                                  reference_source, test_source, MLE_values, RMSE_values);
      // concatenate the vectors to the "all" vectors
      all_reference_source.insert(all_reference_source.end(), reference_source.begin(), reference_source.end() );
      all_test_source.insert(all_test_source.end(), test_source.begin(), test_source.end() );
      all_MLE_values.insert(all_MLE_values.end(), MLE_values.begin(), MLE_values.end() );
      all_RMSE_values.insert(all_RMSE_values.end(), RMSE_values.begin(), RMSE_values.end() );
      all_basin_keys.insert(all_basin_keys.end(), reference_source.size(), basin_key);

      tot_MLE_vec.push_back(tot_MLE);
      cout << "basin: " << basin_key << " and tot_MLE: " << tot_MLE << endl;
    }

    // add the data to the vecvecs
    MLE_vecvec.push_back(all_MLE_values);
    RMSE_vecvec.push_back(all_RMSE_values);
    total_MLE_vecvec.push_back(tot_MLE_vec);
    reference_keys = all_reference_source;
    test_keys = all_test_source;

    // now print the data to the file
    movern_stats_out << "basin_key,reference_source_key,test_source_key,MLE,RMSE" << endl;
    int n_rmse_vals = int(all_RMSE_values.size());
    for(int i = 0; i<n_rmse_vals; i++)
    {
      movern_stats_out << all_basin_keys[i] << ","
                       << all_reference_source[i] << ","
                       << all_test_source[i] << ","
                       << all_MLE_values[i] << ","
                       << all_RMSE_values[i] << endl;
    }
    movern_stats_out.close();

  }

  stats_by_basin_out << "basin_key,outlet_jn";
  stats_by_basin_out.precision(4);
  for(int i = 0; i< n_movern; i++)
  {
    stats_by_basin_out << ",m_over_n = "<<movern[i];
  }
  stats_by_basin_out << endl;
  stats_by_basin_out.precision(9);
  for(int basin_key = 0; basin_key<n_basins; basin_key++)
  {
    stats_by_basin_out << basin_key << "," << outlet_jns[basin_key];
    for(int i = 0; i< n_movern; i++)
    {
      stats_by_basin_out << "," <<total_MLE_vecvec[i][basin_key];
    }
    stats_by_basin_out << endl;
  }

  stats_by_basin_out.close();

}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function test the collinearity of all segments compared to a reference
// segment
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::calculate_goodness_of_fit_collinearity_fxn_movern_with_discharge(LSDFlowInfo& FlowInfo,
                        LSDJunctionNetwork& JN, float start_movern, float delta_movern, int n_movern,
                        bool only_use_mainstem_as_reference,
                        string file_prefix,
                        LSDRaster& Discharge)
{
  // these vectors store all the values which are then used for printing
  vector< vector<float> > RMSE_vecvec;
  vector< vector<float> > MLE_vecvec;
  vector< vector<float> > total_MLE_vecvec;
  vector<int> reference_keys;
  vector<int> test_keys;
  vector<int> outlet_jns;


  cout << "LSDChiTools::calculate_goodness_of_fit_collinearity_fxn_movern" << endl;
  cout << "I am defaulting to A_0 = 1." << endl;
  vector<float> movern;
  float A_0 = 1;
  //float thresh_area_for_chi = 0;      // This just gets chi from all pixels.

  string filename_bstats = file_prefix+"_basinstats.csv";
  ofstream stats_by_basin_out;
  stats_by_basin_out.open(filename_bstats.c_str());

  int n_basins = int(ordered_baselevel_nodes.size());

  // get the outlet junction of each basin key
  for (int basin_key = 0; basin_key < n_basins; basin_key++)
  {
    int outlet_node = ordered_baselevel_nodes[basin_key];
    int outlet_jn = JN.get_Junction_of_Node(outlet_node, FlowInfo);
    outlet_jns.push_back(outlet_jn);
  }

  cout << endl << endl << "==========================" << endl;
  for(int i = 0; i< n_movern; i++)
  {
    // get the m over n value
    movern.push_back( float(i)*delta_movern+start_movern );
    cout << "i: " << i << " and m over n: " << movern[i] << " ";

    // open the outfile
    string filename_fullstats = file_prefix+"_"+dtoa(movern[i])+"_fullstats.csv";
    ofstream movern_stats_out;
    movern_stats_out.open(filename_fullstats.c_str());

    // calculate chi
    float area_threshold = 0;

    LSDRaster this_chi = FlowInfo.get_upslope_chi_from_all_baselevel_nodes(movern[i], A_0,
                                 area_threshold, Discharge);
    update_chi_data_map(FlowInfo, this_chi);

    // these are the vectors that will hold the information about the
    // comparison between channels.
    // the _all vectors are one of all the basins
    // reference source is the source key of the reference channel
    vector<int> reference_source, all_reference_source;
    // test source is the source key of the test channel
    vector<int> test_source, all_test_source;
    // MLE the maximum liklihood estimator
    vector<float> MLE_values, all_MLE_values;
    // RMSE is the root mean square error
    vector<float> RMSE_values, all_RMSE_values;

    vector<float> tot_MLE_vec;

    // now run the collinearity test
    float tot_MLE;

    for(int basin_key = 0; basin_key<n_basins; basin_key++)
    {
      tot_MLE = test_all_segment_collinearity_by_basin(FlowInfo, only_use_mainstem_as_reference,
                                  basin_key,
                                  reference_source, test_source, MLE_values, RMSE_values);
      // concatenate the vectors to the "all" vectors
      all_reference_source.insert(all_reference_source.end(), reference_source.begin(), reference_source.end() );
      all_test_source.insert(all_test_source.end(), test_source.begin(), test_source.end() );
      all_MLE_values.insert(all_MLE_values.end(), MLE_values.begin(), MLE_values.end() );
      all_RMSE_values.insert(all_RMSE_values.end(), RMSE_values.begin(), RMSE_values.end() );

      tot_MLE_vec.push_back(tot_MLE);
      cout << "basin: " << basin_key << " and tot_MLE: " << tot_MLE << endl;
    }

    // add the data to the vecvecs
    MLE_vecvec.push_back(all_MLE_values);
    RMSE_vecvec.push_back(all_RMSE_values);
    total_MLE_vecvec.push_back(tot_MLE_vec);
    reference_keys = all_reference_source;
    test_keys = all_test_source;

    // now print the data to the file
    movern_stats_out << "reference_source_key,test_source_key,MLE,RMSE" << endl;
    int n_rmse_vals = int(all_RMSE_values.size());
    for(int i = 0; i<n_rmse_vals; i++)
    {
      movern_stats_out << all_reference_source[i] << ","
                       << all_test_source[i] << ","
                       << all_MLE_values[i] << ","
                       << all_RMSE_values[i] << endl;
    }
    movern_stats_out.close();

  }

  stats_by_basin_out << "basin_key,outlet_jn";
  stats_by_basin_out.precision(4);
  for(int i = 0; i< n_movern; i++)
  {
    stats_by_basin_out << ",m_over_n = "<<movern[i];
  }
  stats_by_basin_out << endl;
  stats_by_basin_out.precision(9);
  for(int basin_key = 0; basin_key<n_basins; basin_key++)
  {
    stats_by_basin_out << basin_key << "," << outlet_jns[basin_key];
    for(int i = 0; i< n_movern; i++)
    {
      stats_by_basin_out << "," <<total_MLE_vecvec[i][basin_key];
    }
    stats_by_basin_out << endl;
  }

  stats_by_basin_out.close();

}



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This prints a series of simple profiles (chi-elevation) as a function of
// movern
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::print_profiles_as_fxn_movern(LSDFlowInfo& FlowInfo, string filename, float start_movern, float delta_movern, int n_movern)
{
  float A_0 = 1;
  float this_movern;

  vector<float> movern_values;
  vector< vector<float> > chi_vecvec;
  vector<float> empty_vec;
  vector<float> this_chi_vec;
  int this_node;
  int n_nodes = int(node_sequence.size());

  // loop through m over n values
  for(int i = 0; i< n_movern; i++)
  {

    this_movern =  float(i)*delta_movern+start_movern;
    update_chi_data_map(FlowInfo, A_0, this_movern);

    cout << "m/n is: " << this_movern << endl;

    movern_values.push_back(this_movern);
    this_chi_vec = empty_vec;

    // now get the chi values for each node and push them into the chi_vecvec
    for (int n = 0; n< n_nodes; n++)
    {
      this_node = node_sequence[n];
      this_chi_vec.push_back(chi_data_map[this_node]);
    }
    chi_vecvec.push_back(this_chi_vec);
  }
  cout << "Okay, I've got all the chi values in the vecvec." << endl;
  // okay, we are done getting all the chi values, now add these into the file

  ofstream chi_csv_out;
  cout << "Running the printing for movern. Filename is: " << filename << endl;
  chi_csv_out.open(filename.c_str());
  chi_csv_out << "source_key,basin_key,elevation";
  for (int i = 0; i< n_movern; i++)
  {
    chi_csv_out << ",m_over_n = " << movern_values[i];
  }
  chi_csv_out << endl;

  // now loop through all the nodes
  chi_csv_out.precision(5);
  for (int n = 0; n< n_nodes; n++)
  {
    this_node = node_sequence[n];

    chi_csv_out << source_keys_map[this_node] << ","
                 << baselevel_keys_map[this_node] << ","
                 << elev_data_map[this_node];

    for (int i = 0; i< n_movern; i++)
    {
      chi_csv_out << "," << chi_vecvec[i][n];
    }
    chi_csv_out << endl;
  }
  chi_csv_out.close();
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This prints a series of simple profiles (chi-elevation) as a function of
// movern
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::print_profiles_as_fxn_movern_with_discharge(LSDFlowInfo& FlowInfo, string filename,
           float start_movern, float delta_movern, int n_movern, LSDRaster& Discharge)
{
  float A_0 = 1.0;
  float this_movern;

  vector<float> movern_values;
  vector< vector<float> > chi_vecvec;
  vector<float> empty_vec;
  vector<float> this_chi_vec;
  int this_node;
  int n_nodes = int(node_sequence.size());

  // loop through m over n values
  for(int i = 0; i< n_movern; i++)
  {

    this_movern =  float(i)*delta_movern+start_movern;

    // calculate chi
    float area_threshold = 0;

    LSDRaster this_chi = FlowInfo.get_upslope_chi_from_all_baselevel_nodes(this_movern,A_0,
                                 area_threshold, Discharge);
    update_chi_data_map(FlowInfo, this_chi);

    cout << "m/n is: " << this_movern << endl;

    movern_values.push_back(this_movern);
    this_chi_vec = empty_vec;

    // now get the chi values for each node and push them into the chi_vecvec
    for (int n = 0; n< n_nodes; n++)
    {
      this_node = node_sequence[n];
      this_chi_vec.push_back(chi_data_map[this_node]);
    }
    chi_vecvec.push_back(this_chi_vec);
  }
  cout << "Okay, I've got all the chi values in the vecvec." << endl;
  // okay, we are done getting all the chi values, now add these into the file

  ofstream chi_csv_out;
  cout << "Running the printing for movern. Filename is: " << filename << endl;
  chi_csv_out.open(filename.c_str());
  chi_csv_out << "source_key,basin_key,elevation";
  for (int i = 0; i< n_movern; i++)
  {
    chi_csv_out << ",m_over_n = " << movern_values[i];
  }
  chi_csv_out << endl;

  // now loop through all the nodes
  chi_csv_out.precision(5);
  for (int n = 0; n< n_nodes; n++)
  {
    this_node = node_sequence[n];

    chi_csv_out << source_keys_map[this_node] << ","
                 << baselevel_keys_map[this_node] << ","
                 << elev_data_map[this_node];

    for (int i = 0; i< n_movern; i++)
    {
      chi_csv_out << "," << chi_vecvec[i][n];
    }
    chi_csv_out << endl;
  }
  chi_csv_out.close();
}



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This gets the source node of a given key
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int LSDChiTools::get_source_from_source_key(int source_key)
{
  // first get the source node of the reference channel
  int source_node = -9999;
  map<int,int>::iterator iter = key_to_source_map.begin();
  while(iter != key_to_source_map.end())
  {
    if (iter->second == source_key)
    {
      source_node = iter->first;
      //cout << "I found the source key AWESOME! The source key is: " << source_node << endl;
    }
    iter++;
  }

  if ( source_node == -9999 )
  {
    cout << "LSDChiTools::get_starting_node_of_source " << endl;
    cout << "FATAL ERROR: This source is not in the channel network. Source is: " << source_key << endl;
    exit(EXIT_FAILURE);
  }

  return source_node;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Given a source key, find the index of the starting node in the node sequence
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int LSDChiTools::get_starting_node_of_source(int source_key)
{
  int source_node = get_source_from_source_key(source_key);
  int this_starting_node = -9999;

  // Find the node sequence index of this node;
  int this_ns_node = -1;

  bool not_starting_node = true;
  while (not_starting_node)
  {
    this_ns_node++;
    if(node_sequence[this_ns_node] == source_node)
    {
      not_starting_node = false;
      this_starting_node = this_ns_node;
    }
  }
  //cout << "The starting node in the sequence is: " << this_starting_node << endl;

  return this_starting_node;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Gets the number of channels
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int LSDChiTools::get_number_of_channels()
{
  int n_channels = int(key_to_source_map.size());
  return n_channels;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Extract the elevation and chi data from a channel
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::get_chi_elevation_data_of_channel(LSDFlowInfo& FlowInfo, int source_key,
                                vector<float>& chi_data, vector<float>& elevation_data)
{
  int n_channels = int(key_to_source_map.size());
  if (source_key>=n_channels)
  {
    cout << "LSDChiTools::get_chi_elevation_data_of_channel FATAL ERROR" << endl;
    cout << "This source key is not in channel network" << endl;
    exit(EXIT_FAILURE);
  }

  // get source node
  int starting_source = get_source_from_source_key(source_key);
  //cout << "LSDChiTools::get_chi_elevation_data_of_channel, Starting source is: " << starting_source << endl;

  vector<float> this_chi;
  vector<float> this_elevation;

  // add the source to the chi elevation vectors
  this_chi.push_back(chi_data_map[starting_source]);
  this_elevation.push_back(elev_data_map[starting_source]);

  //cout << "Starting chi is: " << chi_data_map[starting_source] << endl;

  // now work downstream until you get to a different source or
  // a baselevel node
  bool is_end = false;
  int current_node = starting_source;
  int receiver_node,receiver_row,receiver_col;
  int this_source_key;
  while(not is_end)
  {
    FlowInfo.retrieve_receiver_information(current_node,receiver_node, receiver_row,receiver_col);

    if(current_node == receiver_node)
    {
      // this is a baselelvel node, simply switch on the is_end boolean
      is_end = true;
    }
    else
    {
      this_source_key = source_keys_map[receiver_node];
      if (this_source_key != source_key)
      {
        //cout << "I made it to the end of this channel" << endl;
      }
      else
      {
        this_chi.push_back(chi_data_map[receiver_node]);
        this_elevation.push_back(elev_data_map[receiver_node]);
      }
    }
    // increment the node downstream
    current_node = receiver_node;

  }

  elevation_data = this_elevation;
  chi_data = this_chi;
  //cout << "Starting chi is: " << chi_data[0] << " and ending chi is: " << chi_data[n_nodes-1] << endl;

  // For debugging
  //int n_nodes = int(elevation_data.size());
  //for(int i= 0; i<n_nodes; i++)
  //{
  //  cout << chi_data[i] << "," << elevation_data[i] << endl;
  //}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Project data onto a reference chi-elevation profile
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
vector<float> LSDChiTools::project_data_onto_reference_channel(vector<float>& reference_chi,
                                 vector<float>& reference_elevation, vector<float>& trib_chi,
                                 vector<float>& trib_elevation)
{
  // How this works is that you take the tributary elevations and then
  // determine the elevation on the reference at the same chi. This is done by
  // interpolating the elevation as a linear fit between the two adjacent chi
  // points on the reference channel.
  vector<float> joint_chi;
  vector<float> trib_joint_elev;
  vector<float> ref_joint_elev;
  vector<float> residuals;


  float this_chi;
  float max_ref_chi;
  float min_ref_chi;
  float max_trib_chi;
  float min_trib_chi;

  int n_ref_nodes = int(reference_chi.size());
  int n_trib_nodes = int(trib_chi.size());
  if (n_ref_nodes <= 1 || n_trib_nodes <= 1)
  {
    cout << "LSDChiTools::project_data_onto_reference_channel WARNING" << endl;
    cout << "The reference channel has 1 or zero nodes." << endl;
    return residuals;
  }
  else
  {
    max_ref_chi = reference_chi[0];
    min_ref_chi = reference_chi[n_ref_nodes-1];

    max_trib_chi = trib_chi[0];
    min_trib_chi = trib_chi[n_trib_nodes-1];
  }

  // test to see if there is overlap
  if(min_trib_chi > max_ref_chi || max_trib_chi < min_ref_chi)
  {
    cout << "LSDChiTools::project_data_onto_reference_channel These channels do not overlap." << endl;
    return residuals;
  }

  // The reference chis monotonically decrease so we will keep track of what
  // indices the bounding chi points are.
  int start_ref_index = 0;
  int end_ref_index = 1;

  // begin by ramping up to the first node within the reference vector
  float this_trib_chi = trib_chi[0];
  int this_node=0;
  if(this_trib_chi > max_ref_chi)
  {
    // the node in the trib is outside of the reference frame. Increment
    // the index until it is.
    while(this_node < n_trib_nodes && trib_chi[this_node] <= max_ref_chi)
    {
      this_node++;
    }
  }

  this_chi = trib_chi[this_node];
  float ref_chi_upstream = reference_chi[start_ref_index];
  float ref_chi_downstream =  reference_chi[end_ref_index];

  //cout << "The number of trib nodes is: " << n_trib_nodes << endl;
  // now ramp up the the start ref index and end ramp index
  bool found_joint_chi;
  for (int i = this_node; i<n_trib_nodes; i++)
  {
    found_joint_chi = false;
    //cout << "trib node: " << i << endl;
    // Get the chi for this node
    this_chi = trib_chi[i];

    // now test if it is between the upstream and downstream chi coordinates in the reference vector
    if (this_chi < ref_chi_upstream && this_chi > ref_chi_downstream)
    {
      // It is between these reference chi values!
      //cout << "FOUND CHI This chi is: " << this_chi << " and bounds are: " << ref_chi_upstream << "," << ref_chi_downstream << endl;
      found_joint_chi = true;
    }
    else
    {
      // we didn't find the chi, we need to move through the reference vector to find
      // the chi value
      bool found_ref_nodes = false;
      while (end_ref_index < n_ref_nodes && not found_ref_nodes)
      {
        start_ref_index++;
        end_ref_index++;
        ref_chi_upstream = reference_chi[start_ref_index];
        ref_chi_downstream =  reference_chi[end_ref_index];
        if (this_chi < ref_chi_upstream && this_chi > ref_chi_downstream)
        {
          found_ref_nodes = true;
          found_joint_chi = true;
          //cout << "FOUND CHI This chi is: " << this_chi << " and bounds are: " << ref_chi_upstream << "," << ref_chi_downstream << endl;
        }
      }
      // There is different logic if we reached the end of the reference vector
      if (end_ref_index == n_ref_nodes-1)
      {
        //cout << "I am at the end of the reference vector" << endl;
        i = n_trib_nodes-1;
      }
    }

    if(found_joint_chi)
    {
      // we need to calculate the eleavtion on the reference vector
      float dist_ref = ref_chi_upstream-ref_chi_downstream;
      float chi_frac = (this_chi-ref_chi_downstream)/dist_ref;
      float joint_elev = chi_frac*(reference_elevation[start_ref_index]-reference_elevation[end_ref_index])
                             +reference_elevation[end_ref_index];
      joint_chi.push_back(this_chi);
      trib_joint_elev.push_back(trib_elevation[i]);
      ref_joint_elev.push_back(joint_elev);
      residuals.push_back(trib_elevation[i]-joint_elev);
    }

  }

  //for(int i = 0; i<int(joint_chi.size()); i++)
  //{
  //  cout << "residual[" << i << "]: "<< residuals[i] << endl;
  //}

  // this section is for debugging
  bool print_for_debugging = false;
  if (print_for_debugging)
  {
    ofstream chans_out;
    chans_out.open("Test_project.csv");
    chans_out << "chi,elev,channel_code" << endl;
    for(int i = 0; i<n_ref_nodes; i++)
    {
      chans_out << reference_chi[i] << "," << reference_elevation[i] <<",0" <<endl;
    }
    for(int i = 0; i<n_trib_nodes; i++)
    {
      chans_out << trib_chi[i] << "," << trib_elevation[i] <<",1" <<endl;
    }
    for(int i = 0; i< int(joint_chi.size()); i++)
    {
      chans_out << joint_chi[i] << "," << trib_joint_elev[i] <<",2" <<endl;
    }
    for(int i = 0; i<int(joint_chi.size()); i++)
    {
      chans_out << joint_chi[i] << "," << ref_joint_elev[i] <<",3" <<endl;
    }
    chans_out.close();
  }


  return residuals;

}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This does slope-area analysis
// The strategy is to loop through each source
// progress down each but do not cross tributary junctions
// The data is not at all the points since we are averaging across multiple
// pixels.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::get_slope_area_data(LSDFlowInfo& FlowInfo, float vertical_interval,
                                  vector<int>& midpoint_nodes, vector<float>& slopes)
{
  // set verbose to true if you want to print the data as you go along
  bool verbose = false;

  // these are the vectors that will hold all the data
  // Note we don't need to keep track of all the area, elevation, location, etc.
  // because these are all keyed to to the node index of the midpoint.
  // The only thing we need to retain is the slope.
  vector<float> SA_slope;
  vector<int> SA_midpoint_node;

  // used to calculate the S-A data
  float half_interval = vertical_interval*0.5;
  //float midpoint_area;
  float upstream_elevation;
  float upstream_flow_distance;
  float downstream_elevation;
  float downstream_flow_distance;
  float midpoint_node;
  float target_end_interval_elevation;
  float target_midpoint_interval_elevation;

  // we loop through every source
  int n_sources = int(ordered_source_nodes.size());
  if(verbose)
  {
    cout << "Super times, you are going to do a S-A analysis." << endl;
    cout << "Number of sources is: " << n_sources << endl;
  }


  int top_interval_node;
  int search_node;
  int this_source_node;
  int this_source_key;
  int row,col;
  for(int s = 0; s<n_sources; s++)
  {
    top_interval_node = ordered_source_nodes[s];
    this_source_node = top_interval_node;
    this_source_key = source_keys_map[this_source_node];
    // now trace downstream until you first get to the midpoint,
    // and then to the final node.
    bool is_this_final_node = false;
    bool is_end_interval;
    bool is_midpoint_interval;
    int last_node;

    midpoint_node = 0;    // this will be updated later

    if (verbose)
    {
      cout << "=====================" << endl;
      cout << "I am starting on a channel with source node " << this_source_node << " and key: " << this_source_key << endl;
    }

    // now, start at the top node of each channel and
    // work down. The algorithm looks downstream until it hits
    // the midpoint, and then continues until it hits
    // the end point
    // if it encounters the end of the channel before it hits
    // the end point the loop exits
    while (not is_this_final_node)
    {
      // get the upstream elevation and flow distance
      upstream_elevation = elev_data_map[top_interval_node];
      upstream_flow_distance = flow_distance_data_map[top_interval_node];

      target_end_interval_elevation = upstream_elevation-vertical_interval;
      target_midpoint_interval_elevation = upstream_elevation-half_interval;

      if(verbose)
      {
        cout << "Looking for a midpoint for top interval node: " << top_interval_node << endl;
        cout << "Top z: " << upstream_elevation << " and top fd: " << upstream_flow_distance << endl;
      }

      // this gets the receiver (placed into the seach node)
      FlowInfo.retrieve_receiver_information(top_interval_node,
                     search_node, row, col);

      // check to see if this is the last element or in a tributary
      if (search_node == top_interval_node || this_source_key != source_keys_map[search_node])
      {
        is_this_final_node = true;
      }
      else
      {
        // reset midpoint and end flags
        is_end_interval = false;
        is_midpoint_interval = false;

        // now work downstream
        while (not is_this_final_node && not is_end_interval)
        {
          //cout << "search_node: " << search_node << " elev: " << elevations[chan][search_node]
          //     << " and target mp, end: " << target_mp_interval_elevations << " " << target_end_interval_elevations << endl;

          // see if search node is the midpoint node
          if ( elev_data_map[search_node] <= target_midpoint_interval_elevation && not is_midpoint_interval)
          {
            //midpoint_area = drainage_area_data_map[search_node];
            midpoint_node = search_node;

            // set midpoint flag so it doens't collect downstream nodes
            is_midpoint_interval = true;

            if(verbose)
            {
              cout << endl << endl << "=========" << endl << "I found the midpoint!" << endl;
            }
          }

          // see if the search node is the end node
          if (elev_data_map[search_node] <= target_end_interval_elevation)
          {
            downstream_elevation = elev_data_map[search_node];
            downstream_flow_distance = flow_distance_data_map[search_node];

            // make sure the code knows this is the end, the only end, my friend.
            is_end_interval = true;;
          }

          // now move downstream
          last_node = search_node;
          FlowInfo.retrieve_receiver_information(last_node,
                     search_node, row, col);

          // test is this is the end
          if (search_node == last_node || this_source_key != source_keys_map[search_node])
          {
            is_this_final_node = true;
          }

        }

        // if is_end_interval is true, that means it found the end interval.
        // record the information. If it didn't reach the end flag that means
        // the previous node hit the final node in the channel before finding the
        // end interval.
        if (is_end_interval)
        {
          float slope = (upstream_elevation-downstream_elevation)/
                       (upstream_flow_distance-downstream_flow_distance);


          // It the slope is zero or less than zero we ignore the data
          if(slope > 0)
          {
            // record the data
            SA_midpoint_node.push_back(midpoint_node);
            SA_slope.push_back(slope);
          }
        }             // if statement for recording data on S-A to data containers
      }               // end check if tributary only has one node
      // now we need to update the top interval node
      int old_top_interval = top_interval_node;
      FlowInfo.retrieve_receiver_information(old_top_interval,
                     top_interval_node, row, col);
      if(verbose)
      {
        cout << "Resetting the top interval node, old:  " << old_top_interval << " and new: " << top_interval_node << endl;
      }

    }                 // check if this is the final node of the source trib
  }                   // end sources loop (at this point we go to the next source)

  midpoint_nodes = SA_midpoint_node;
  slopes = SA_slope;

}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This bins the data
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::bin_slope_area_data(LSDFlowInfo& FlowInfo,
                                          vector<int>& SA_midpoint_node,
                                          vector<float>& SA_slope,
                                          float log_bin_width,
                                          string filename)
{

  vector<float> empty_vec;

  // we will store the data in maps where the key is the source node
  // This is because we will bin the data by source.
  //map< int, vector<float> > slope;
  //map< int, vector<float> > area;
  map< int, vector<float> > log_slope_map;
  map< int, vector<float> > log_area_map;
  map< int, int > basin_key_of_this_source_map;

  int this_node;
  int this_source_key;

  int n_nodes = int(SA_midpoint_node.size());
  if (n_nodes <= 0)
  {
    cout << "Trying to print SA data but there doesn't seem to be any." << endl;
    cout << "Have you run the automator and gathered the sources yet?" << endl;
  }
  else
  {
    // get the data vectors out
    for (int n = 0; n< n_nodes; n++)
    {
      // get the source node
      this_node = SA_midpoint_node[n];
      this_source_key = source_keys_map[this_node];
      //cout << "This source key is: " << this_source_key << endl;

      // see if we have a vector for that source node
      if( log_area_map.find(this_source_key) == log_area_map.end())
      {
        log_area_map[this_source_key] = empty_vec;
        log_slope_map[this_source_key] = empty_vec;
      }
      // check if we have the basin of this source
      if (basin_key_of_this_source_map.find(this_source_key) == basin_key_of_this_source_map.end() )
      {
        basin_key_of_this_source_map[this_source_key] = baselevel_keys_map[this_node];
      }

      // add to this source's log S, log A data. We will later use these to bin
      log_area_map[this_source_key].push_back( log10(drainage_area_data_map[this_node]) );
      log_slope_map[this_source_key].push_back( log10(SA_slope[n]) );
    }
  }

  // now we bin the data
  //float bin_width = 0.1;
  vector<float>  MeanX_output;
  vector<float> MeanY_output;
  vector<float> midpoints_output;
  vector<float> MedianY_output;
  vector<float> StandardDeviationX_output;
  vector<float> StandardDeviationY_output;
  vector<float> StandardErrorX_output;
  vector<float> StandardErrorY_output;
  vector<int> number_observations_output;
  float bin_lower_limit;
  float NoDataValue = -9999;

  // these are the vectors holding all the compiled information
  vector<int> binned_basin_keys;
  vector<int> binned_source_keys;
  vector<float> binned_logA_means;
  vector<float> binned_logA_midpoints;
  vector<float> binned_logS_means;
  vector<float> binned_logS_medians;
  vector<float> binned_logA_stdErr;
  vector<float> binned_logS_stdErr;
  vector<int> binnned_NObvs;

  // loop through all the source nodes
  map<int, vector<float> >::iterator it;
  for(it = log_area_map.begin(); it != log_area_map.end(); ++it)
  {
    this_source_key =  it->first;

    //cout << "The source key is: " << this_source_key << endl;

    // extract the log S-log A data for this source
    vector<float> log_area = log_area_map[this_source_key];
    vector<float> log_slope = log_slope_map[this_source_key];

    // this gets the binned data for this particular tributary
    bin_data(log_area, log_slope, log_bin_width,  MeanX_output, MeanY_output,
            midpoints_output, MedianY_output,StandardDeviationX_output,
            StandardDeviationY_output, StandardErrorX_output, StandardErrorY_output,
            number_observations_output, bin_lower_limit, NoDataValue);

    // now we need to add this information to the master vectors
    int n_bins = int(midpoints_output.size());
    int n_Obvs;
    for(int i = 0; i<n_bins; i++)
    {
      n_Obvs = number_observations_output[i];

      // only record data if there are enough observations
      if(n_Obvs > 0)
      {
        binned_basin_keys.push_back(basin_key_of_this_source_map[this_source_key]);
        binned_source_keys.push_back(this_source_key);
        binned_logA_means.push_back(MeanX_output[i]);
        binned_logA_midpoints.push_back(midpoints_output[i]);
        binned_logS_means.push_back(MeanY_output[i]);
        binned_logS_medians.push_back(MedianY_output[i]);
        binned_logA_stdErr.push_back(StandardErrorX_output[i]);
        binned_logS_stdErr.push_back(StandardErrorY_output[i]);
        binnned_NObvs.push_back(n_Obvs);
      }
    }
  }


  // now print to file
  int n_data_points = int(binnned_NObvs.size());
  ofstream  binned_out;
  binned_out.open(filename.c_str());
  binned_out << "basin_key,source_key,midpoints_log_A,mean_log_A,mean_log_S,median_log_S,logA_stdErr,logS_stdErr,n_observations" << endl;
  for(int i = 0; i<n_data_points; i++)
  {
    binned_out << binned_basin_keys[i] << ","
               << binned_source_keys[i] << ","
               << binned_logA_midpoints[i] << ","
               << binned_logA_means[i] << ","
               << binned_logS_means[i] << ","
               << binned_logS_medians[i] << ","
               << binned_logA_stdErr[i] << ","
               << binned_logS_stdErr[i] << ","
               << binnned_NObvs[i] << endl;
  }
  binned_out.close();
}



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Print S-A data maps to file
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::print_slope_area_data_to_csv(LSDFlowInfo& FlowInfo,
                                          vector<int>& SA_midpoint_node,
                                          vector<float>& SA_slope,
                                          string filename)
{
  // open the data file
  int row,col;
  LSDCoordinateConverterLLandUTM Converter;
  int n_nodes = int(SA_midpoint_node.size());
  double latitude,longitude;
  ofstream  SA_out;
  SA_out.open(filename.c_str());
  cout << "Opening the data file: " << filename << endl;
  SA_out << "latitude,longitude,chi,elevation,flow distance,drainage area,slope,source_key,basin_key" << endl;
  int this_node;
  if (n_nodes <= 0)
  {
    cout << "Trying to print SA data but there doesn't seem to be any." << endl;
    cout << "Have you run the automator and gathered the sources yet?" << endl;
  }
  else
  {
    for (int n = 0; n< n_nodes; n++)
    {
      this_node = SA_midpoint_node[n];
      FlowInfo.retrieve_current_row_and_col(this_node,row,col);
      get_lat_and_long_locations(row, col, latitude, longitude, Converter);

      SA_out.precision(9);
      SA_out << latitude << ","
             << longitude << ",";
      SA_out.precision(5);
      SA_out << chi_data_map[this_node] << ","
             << elev_data_map[this_node] << ","
             << flow_distance_data_map[this_node] << ","
             << drainage_area_data_map[this_node] << ","
             << SA_slope[n] << ","
             << source_keys_map[this_node] << ","
             << baselevel_keys_map[this_node];
      SA_out << endl;
    }
  }

  SA_out.close();

}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Print chi maps to file
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::print_chi_data_map_to_csv(LSDFlowInfo& FlowInfo, string filename)
{

  // these are for extracting element-wise data from the channel profiles.
  int this_node, row,col;
  double latitude,longitude;
  LSDCoordinateConverterLLandUTM Converter;

  // find the number of nodes
  int n_nodes = (node_sequence.size());

  // open the data file
  ofstream  chi_data_out;
  chi_data_out.open(filename.c_str());
  chi_data_out << "latitude,longitude,chi,elevation,flow distance,drainage area,source_key,basin_key" << endl;
  if (n_nodes <= 0)
  {
    cout << "Cannot print since you have not calculated channel properties yet." << endl;
  }
  else
  {
    for (int n = 0; n< n_nodes; n++)
    {
      this_node = node_sequence[n];
      FlowInfo.retrieve_current_row_and_col(this_node,row,col);
      get_lat_and_long_locations(row, col, latitude, longitude, Converter);

      chi_data_out.precision(9);
      chi_data_out << latitude << ","
                   << longitude << ",";
      chi_data_out.precision(5);
      chi_data_out << chi_data_map[this_node] << ","
                   << elev_data_map[this_node] << ","
                   << flow_distance_data_map[this_node] << ","
                   << drainage_area_data_map[this_node] << ","
                   << source_keys_map[this_node] << ","
                   << baselevel_keys_map[this_node];

      chi_data_out << endl;
    }
  }

  chi_data_out.close();

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Print data maps to file
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::print_data_maps_to_file_full(LSDFlowInfo& FlowInfo, string filename)
{

  // these are for extracting element-wise data from the channel profiles.
  int this_node, row, col;
  double latitude,longitude;
  LSDCoordinateConverterLLandUTM Converter;

  // find the number of nodes
  int n_nodes = (node_sequence.size());

  // test to see if there is segment numbering
  bool have_segments = false;
  if( segment_counter_map.size() == node_sequence.size())
  {
    have_segments = true;
  }

  // test to see if the fitted elevations have been calculated
  bool have_segmented_elevation = false;
  if( segmented_elevation_map.size() == node_sequence.size())
  {
    have_segmented_elevation = true;
  }


  // open the data file
  ofstream  chi_data_out;
  chi_data_out.open(filename.c_str());
  chi_data_out << "node,row,col,latitude,longitude,chi,elevation,flow distance,drainage area,m_chi,b_chi,source_key,basin_key";
  if(have_segmented_elevation)
  {
    chi_data_out << ",segmented_elevation";
  }
  if (have_segments)
  {
    chi_data_out << ",segment_number";
    cout << "I added the segment number in the csv file"<< endl;
  }
  chi_data_out << endl;




  if (n_nodes <= 0)
  {
    cout << "Cannot print since you have not calculated channel properties yet." << endl;
  }
  else
  {
    for (int n = 0; n< n_nodes; n++)
    {
      this_node = node_sequence[n];
      FlowInfo.retrieve_current_row_and_col(this_node,row,col);
      get_lat_and_long_locations(row, col, latitude, longitude, Converter);

      chi_data_out << this_node << ","
                   << row << ","
                   << col << ",";
      chi_data_out.precision(9);
      chi_data_out << latitude << ","
                   << longitude << ",";
      chi_data_out.precision(5);
      chi_data_out << chi_data_map[this_node] << ","
                   << elev_data_map[this_node] << ","
                   << flow_distance_data_map[this_node] << ","
                   << drainage_area_data_map[this_node] << ","
                   << M_chi_data_map[this_node] << ","
                   << b_chi_data_map[this_node] << ","
                   << source_keys_map[this_node] << ","
                   << baselevel_keys_map[this_node];

      if(have_segmented_elevation)
      {
        chi_data_out << "," << segmented_elevation_map[this_node];
      }
      if (have_segments)
      {
        chi_data_out << "," << segment_counter_map[this_node];
      }
      chi_data_out << endl;
    }
  }

  chi_data_out.close();

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Development function to Print data maps to file including knickpoints
// BG
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::print_data_maps_to_file_full_knickpoints(LSDFlowInfo& FlowInfo, string filename)
{

  // these are for extracting element-wise data from the channel profiles.
  int this_node, row,col;
  double latitude,longitude;
  LSDCoordinateConverterLLandUTM Converter;

  // find the number of nodes
  int n_nodes = (node_sequence.size());

  // test to see if there is segment numbering
  bool have_segments = false;
  if( segment_counter_map.size() == node_sequence.size())
  {
    have_segments = true;
  }

  // test to see if the fitted elevations have been calculated
  bool have_segmented_elevation = false;
  if( segmented_elevation_map.size() == node_sequence.size())
  {
    have_segmented_elevation = true;
  }


  // open the data file
  ofstream  chi_data_out;
  chi_data_out.open(filename.c_str());
  chi_data_out << "latitude,longitude,chi,elevation,flow distance,drainage area,m_chi,b_chi,source_key,basin_key";
  if(have_segmented_elevation)
  {
    chi_data_out << ",segmented_elevation";
  }
  if (have_segments)
  {
    chi_data_out << ",segment_number";
  }
  chi_data_out << ",knickpoints,knickpoint_sign,segment_length"; // add the knickpoint col
  chi_data_out << endl;

  if (n_nodes <= 0)
  {
    cout << "Cannot print since you have not calculated channel properties yet." << endl;
  }
  else
  {
    for (int n = 0; n< n_nodes; n++)
    {
      this_node = node_sequence[n];
      FlowInfo.retrieve_current_row_and_col(this_node,row,col);
      get_lat_and_long_locations(row, col, latitude, longitude, Converter);

      chi_data_out.precision(9);
      chi_data_out << latitude << ","
                   << longitude << ",";
      chi_data_out.precision(5);
      chi_data_out << chi_data_map[this_node] << ","
                   << elev_data_map[this_node] << ","
                   << flow_distance_data_map[this_node] << ","
                   << drainage_area_data_map[this_node] << ","
                   << M_chi_data_map[this_node] << ","
                   << b_chi_data_map[this_node] << ","
                   << source_keys_map[this_node] << ","
                   << baselevel_keys_map[this_node];

      if(have_segmented_elevation)
      {
        chi_data_out << "," << segmented_elevation_map[this_node];
      }
      if (have_segments)
      {
        chi_data_out << "," << segment_counter_map[this_node];
      }

      chi_data_out << "," << segment_counter_knickpoint_map[this_node];
      chi_data_out << "," << segment_knickpoint_sign_map[this_node];
      chi_data_out << "," << segment_length_map[this_node];
      chi_data_out << endl;
    }
  }

  chi_data_out.close();

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Print data maps to file
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::print_source_keys(LSDFlowInfo& FlowInfo, string filename)
{

  // these are for extracting element-wise data from the channel profiles.
  int this_node, row,col, key;
  double latitude,longitude;
  LSDCoordinateConverterLLandUTM Converter;
  map<int, int>::iterator it;

  // open the data file
  ofstream  source_keys_out;
  source_keys_out.open(filename.c_str());
  source_keys_out << "latitude,longitude,source_node,source_key" << endl;

  // loop through the source key map
  for ( it = key_to_source_map.begin(); it != key_to_source_map.end(); it++ )
  {
    key = it->second;
    this_node = it->first;
    FlowInfo.retrieve_current_row_and_col(this_node,row,col);
    get_lat_and_long_locations(row, col, latitude, longitude, Converter);

    source_keys_out.precision(9);
    source_keys_out << latitude << ","
                   << longitude << "," << this_node << ",";
    source_keys_out.precision(5);
    source_keys_out << key << endl;
  }

  source_keys_out.close();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Print data maps to file
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::print_baselevel_keys(LSDFlowInfo& FlowInfo, LSDJunctionNetwork& JunctionNetwork, string filename)
{

  // these are for extracting element-wise data from the channel profiles.
  int this_node, this_junc,row,col, key;
  double latitude,longitude;
  LSDCoordinateConverterLLandUTM Converter;
  map<int, int>::iterator it;

  // open the data file
  ofstream  baselevel_keys_out;
  baselevel_keys_out.open(filename.c_str());
  baselevel_keys_out << "latitude,longitude,baselevel_node,baselevel_junction,baselevel_key" << endl;

  // loop through the source
  for ( it = key_to_baselevel_map.begin(); it != key_to_baselevel_map.end(); it++ )
  {
    key = it->second;
    this_node = it->first;
    this_junc = JunctionNetwork.get_Junction_of_Node(this_node, FlowInfo);
    FlowInfo.retrieve_current_row_and_col(this_node,row,col);
    get_lat_and_long_locations(row, col, latitude, longitude, Converter);

    baselevel_keys_out.precision(9);
    baselevel_keys_out << latitude << ","
                   << longitude << "," << this_node << ",";
    baselevel_keys_out.precision(5);
    baselevel_keys_out << this_junc <<"," << key << endl;
  }

  baselevel_keys_out.close();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function prints the basins and an additional file that has basin centroids
// and labelling information for plotting
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::print_basins(LSDFlowInfo& FlowInfo, LSDJunctionNetwork& JunctionNetwork,
                               vector<int> Junctions, string base_filename)
{
  int N_Juncs = Junctions.size();
  LSDCoordinateConverterLLandUTM Converter;


  // Get some data members for holding basins and the raster
  vector<LSDBasin> AllTheBasins;
  map<int,int> drainage_of_other_basins;
  LSDIndexRaster BasinMasterRaster;

  string basin_raster_name = base_filename+"_AllBasins";
  string basin_info_name = base_filename+"_AllBasinsInfo.csv";

  ofstream basin_info_out;
  basin_info_out.open(basin_info_name.c_str());
  basin_info_out << "latitude,longitude,outlet_latitude,outlet_longitude,outlet_junction,basin_key" << endl;

  // Make sure the full lat-long information is printed
  basin_info_out.precision(9);

  // These store row and column information for converting the outlet and centroid to
  // latitude and longitude
  int centroid_i, centroid_j, outlet_i, outlet_j;
  double out_lat,out_long, cen_lat, cen_long;

  //cout << "I am trying to print basins, found " << N_BaseLevelJuncs << " base levels." << endl;
  // Loop through the junctions
  for(int BN = 0; BN<N_Juncs; BN++)
  {
    //cout << "Getting basin " << BN << " and the junction is: "  << Junctions[BN] << endl;
    LSDBasin thisBasin(Junctions[BN],FlowInfo, JunctionNetwork);
    //cout << "...got it!" << endl;
    AllTheBasins.push_back(thisBasin);

    // This is required if the basins are nested--test the code which numbers
    // to be overwritten by a smaller basin
    drainage_of_other_basins[Junctions[BN]] = thisBasin.get_NumberOfCells();


    // We need to see if we can find the basin key
    int basin_key = -9999;
    
    // need to node index of this junction
    int node_of_junction =  JunctionNetwork.get_Node_of_Junction( Junctions[BN] );
    if ( key_to_baselevel_map.find( node_of_junction) != key_to_baselevel_map.end() )
    {
      basin_key = key_to_baselevel_map[node_of_junction];
    }

    // get the centroid and outlet locations
    centroid_i = thisBasin.get_Centroid_i();
    centroid_j = thisBasin.get_Centroid_j();

    outlet_i = thisBasin.get_Outlet_i();
    outlet_j = thisBasin.get_Outlet_j();

    // Find the latitude and longitude of the outlet and centroid
    get_lat_and_long_locations(centroid_i, centroid_j, cen_lat, cen_long, Converter);
    get_lat_and_long_locations(outlet_i, outlet_j, out_lat, out_long, Converter);

    basin_info_out << cen_lat << "," << cen_long << "," << out_lat << "," << out_long << "," << Junctions[BN] << "," << basin_key << endl;
  }
  basin_info_out.close();

  // now loop through everything again getting the raster
  if (N_Juncs > 0)     // this gets the first raster
  {
    BasinMasterRaster = AllTheBasins[0].write_integer_data_to_LSDIndexRaster(Junctions[0], FlowInfo);
  }

  // now add on the subsequent basins
  for(int BN = 1; BN<N_Juncs; BN++)
  {
    AllTheBasins[BN].add_basin_to_LSDIndexRaster(BasinMasterRaster, FlowInfo,
                              drainage_of_other_basins, Junctions[BN]);
  }


  // We need to use bil format since there is georeferencing
  string raster_ext = "bil";
  // print the basin raster
  BasinMasterRaster.write_raster(basin_raster_name, raster_ext);
  cout << "Finished with exporting basins!" << endl;

}





//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Print data maps to file
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::print_data_maps_to_file_basic(LSDFlowInfo& FlowInfo, string filename)
{

  // these are for extracting element-wise data from the channel profiles.
  int this_node, row,col;
  double latitude,longitude;
  LSDCoordinateConverterLLandUTM Converter;

  // open the data file
  ofstream  chi_data_out;
  chi_data_out.open(filename.c_str());
  chi_data_out << "latitude,longitude,m_chi,b_chi" << endl;

  // find the number of nodes
  int n_nodes = (node_sequence.size());
  if (n_nodes <= 0)
  {
    cout << "Cannot print since you have not calculated channel properties yet." << endl;
  }
  else
  {
    for (int n = 0; n< n_nodes; n++)
    {
      this_node = node_sequence[n];
      FlowInfo.retrieve_current_row_and_col(this_node,row,col);
      get_lat_and_long_locations(row, col, latitude, longitude, Converter);

      chi_data_out.precision(9);
      chi_data_out << latitude << ","
                   << longitude << ",";
      chi_data_out.precision(6);
      chi_data_out << M_chi_data_map[this_node] << ","
                   << b_chi_data_map[this_node] << "," << endl;
    }
  }

  chi_data_out.close();

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-




#endif
