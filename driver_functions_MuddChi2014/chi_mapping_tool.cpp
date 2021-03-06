//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// chi_mapping_tool
//
// This program takes two arguments, the path name and the driver name
// The driver file has a number of options that allow the user to calculate
// different kinds of chi analysis.
//
// The documentation is here:
// https://lsdtopotools.github.io/LSDTopoTools_ChiMudd2014/
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
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
// either version 3 of the License, or (at your option) any later version.
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
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../LSDStatsTools.hpp"
#include "../LSDChiNetwork.hpp"
#include "../LSDRaster.hpp"
#include "../LSDRasterInfo.hpp"
#include "../LSDIndexRaster.hpp"
#include "../LSDFlowInfo.hpp"
#include "../LSDJunctionNetwork.hpp"
#include "../LSDIndexChannelTree.hpp"
#include "../LSDBasin.hpp"
#include "../LSDChiTools.hpp"
#include "../LSDParameterParser.hpp"
#include "../LSDSpatialCSVReader.hpp"
#include "../LSDShapeTools.hpp"

int main (int nNumberofArgs,char *argv[])
{
  //Test for correct input arguments
  if (nNumberofArgs!=3)
  {
    cout << "=========================================================" << endl;
    cout << "|| Welcome to the chi mapping tool!                    ||" << endl;
    cout << "|| This program has a number of options to make chi    ||" << endl;
    cout << "|| plots and to map out slopes in chi space.           ||" << endl;
    cout << "|| This program was developed by Simon M. Mudd         ||" << endl;
    cout << "||  at the University of Edinburgh                     ||" << endl;
    cout << "=========================================================" << endl;
    cout << "This program requires two inputs: " << endl;
    cout << "* First the path to the parameter file." << endl;
    cout << "* Second the name of the param file (see below)." << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << "Then the command line argument will be: " << endl;
    cout << "In linux:" << endl;
    cout << "./chi_mapping_tool.exe /LSDTopoTools/Topographic_projects/LSDTT_chi_examples/ Xian_example1.driver" << endl;
    cout << "=========================================================" << endl;
    cout << "For more documentation on the parameter file, " << endl;
    cout << " see readme and online documentation." << endl;
    cout << " https://lsdtopotools.github.io/LSDTopoTools_ChiMudd2014/" << endl;
    cout << "=========================================================" << endl;
    exit(EXIT_SUCCESS);
  }

  string path_name = argv[1];
  string f_name = argv[2];

  // load parameter parser object
  LSDParameterParser LSDPP(path_name,f_name);

  // for the chi tools we need georeferencing so make sure we are using bil format
  LSDPP.force_bil_extension();

  // maps for setting default parameters
  map<string,int> int_default_map;
  map<string,float> float_default_map;
  map<string,bool> bool_default_map;
  map<string,string> string_default_map;

  // Basic DEM preprocessing
  float_default_map["minimum_elevation"] = 0.0;
  float_default_map["maximum_elevation"] = 30000;
  float_default_map["min_slope_for_fill"] = 0.0001;
  bool_default_map["raster_is_filled"] = false; // assume base raster is already filled
  bool_default_map["remove_seas"] = false; // elevations above minimum and maximum will be changed to nodata
  bool_default_map["only_check_parameters"] = false;
  string_default_map["CHeads_file"] = "NULL";
  

  // Selecting basins
  int_default_map["threshold_contributing_pixels"] = 1000;
  int_default_map["minimum_basin_size_pixels"] = 5000;
  int_default_map["maximum_basin_size_pixels"] = 500000;
  bool_default_map["test_drainage_boundaries"] = true;
  bool_default_map["only_take_largest_basin"] = false;
  string_default_map["BaselevelJunctions_file"] = "NULL";
  
  // IMPORTANT: S-A analysis and chi analysis wont work if you have a truncated
  // basin. For this reason the default is to test for edge effects
  bool_default_map["find_complete_basins_in_window"] = false;
  bool_default_map["find_largest_complete_basins"] = false;
  bool_default_map["print_basin_raster"] = false;

  // printing of rasters and data before chi analysis
  bool_default_map["convert_csv_to_geojson"] = false;  // THis converts all cv files to geojson (for easier loading in a GIS)

  bool_default_map["print_stream_order_raster"] = false;
  bool_default_map["print_channels_to_csv"] = false;
  bool_default_map["print_junction_index_raster"] = false;
  bool_default_map["print_junctions_to_csv"] = false;
  bool_default_map["print_fill_raster"] = false;
  bool_default_map["print_DrainageArea_raster"] = false;
  bool_default_map["write hillshade"] = false;
  bool_default_map["print_basic_M_chi_map_to_csv"] = false;

  // basic parameters for calculating chi
  float_default_map["A_0"] = 1;
  float_default_map["m_over_n"] = 0.5;
  int_default_map["threshold_pixels_for_chi"] = 0;
  int_default_map["basic_Mchi_regression_nodes"];

  // parameters if you want to explore m/n ratios or slope-area analysis
  int_default_map["n_movern"] = 8;
  float_default_map["start_movern"] = 0.1;
  float_default_map["delta_movern"] = 0.1;
  float_default_map["SA_vertical_interval"] = 20;
  float_default_map["log_A_bin_width"] = 0.1;
  bool_default_map["print_slope_area_data"] = false;
  bool_default_map["only_use_mainstem_as_reference"] = true;
  // these loop through m/n spitting out profies and calculating goodness of fit
  // If you want to visualise the data you need to switch both of these to true
  bool_default_map["calculate_MLE_collinearity"] = false;
  bool_default_map["print_profiles_fxn_movern_csv"] = false;

  // parameters for various chi calculations as well as slope-area
  int_default_map["n_iterations"] = 20;
  int_default_map["minimum_segment_length"] = 10;
  int_default_map["n_nodes_to_visit"] = 10;
  int_default_map["target_nodes"] = 80;
  int_default_map["skip"] = 2;
  float_default_map["sigma"] = 20;

  // switches for chi analysis
  // These just print simple chi maps

  bool_default_map["print_chi_coordinate_raster"] = false;
  bool_default_map["print_simple_chi_map_to_csv"] = false;


  // these are routines that run segmentation
  bool_default_map["print_simple_chi_map_with_basins_to_csv"] = false;
  bool_default_map["print_segmented_M_chi_map_to_csv"] = false;
  bool_default_map["print_basic_M_chi_map_to_csv"] = false;

  // these print various basin and source data for visualisation
  bool_default_map["print_source_keys"] = false;
  bool_default_map["print_sources_to_csv"] = false;
  bool_default_map["print_baselevel_keys"] = false;

  // These enable calculation of chi based on discharge
  bool_default_map["use_precipitation_raster_for_chi"] = false;
  bool_default_map["print_discharge_raster"] = false;
  bool_default_map["print_chi_no_discharge"] = false;   // this only is used if you also
                                                        // calculate chi with discharge so you can compare.
  bool_default_map["check_chi_maps"] = false;
  string_default_map["precipitation_fname"] = "NULL";

  bool_default_map["print_segments"] = false;


  // Use the parameter parser to get the maps of the parameters required for the
  // analysis
  LSDPP.parse_all_parameters(float_default_map, int_default_map, bool_default_map,string_default_map);
  map<string,float> this_float_map = LSDPP.get_float_parameters();
  map<string,int> this_int_map = LSDPP.get_int_parameters();
  map<string,bool> this_bool_map = LSDPP.get_bool_parameters();
  map<string,string> this_string_map = LSDPP.get_string_parameters();

  // Now print the parameters for bug checking
  cout << "PRINT THE PARAMETERS..." << endl;
  LSDPP.print_parameters();

  // location of the files
  string DATA_DIR =  LSDPP.get_read_path();
  string DEM_ID =  LSDPP.get_read_fname();
  string OUT_DIR = LSDPP.get_write_path();
  string OUT_ID = LSDPP.get_write_fname();
  string raster_ext =  LSDPP.get_dem_read_extension();
  vector<string> boundary_conditions = LSDPP.get_boundary_conditions();
  string CHeads_file = LSDPP.get_CHeads_file();
  string BaselevelJunctions_file = LSDPP.get_BaselevelJunctions_file();

  cout << "Read filename is: " <<  DATA_DIR+DEM_ID << endl;
  cout << "Write filename is: " << OUT_DIR+OUT_ID << endl;

    // check to see if the raster exists
  LSDRasterInfo RI((DATA_DIR+DEM_ID), raster_ext);

  // check the threshold pixels for chi
  if (this_int_map["threshold_pixels_for_chi"] > this_int_map["threshold_contributing_pixels"])
  {
    cout << "WARNING: you have chosen a threshold pixels for chi which is greater" << endl;
    cout << "   the threshold contributing pixels. Defaulting so these are equal." << endl;
    this_int_map["threshold_pixels_for_chi"] = this_int_map["threshold_contributing_pixels"];
  }

  // initialise variables to be assigned from .driver file
  // These will all be assigned default values
  float A_0 = this_float_map["A_0"];
  float movern = this_float_map["m_over_n"];
  int n_iterations = this_int_map["n_iterations"];
  int minimum_segment_length = this_int_map["minimum_segment_length"];
  int n_nodes_to_visit = this_int_map["n_nodes_to_visit"];             // when constructing channel network, this
  float sigma = this_float_map["sigma"];
  int target_nodes = this_int_map["target_nodes"];
  int skip = this_int_map["skip"];
  int threshold_contributing_pixels = this_int_map["threshold_contributing_pixels"];
  int minimum_basin_size_pixels = this_int_map["minimum_basin_size_pixels"];
  int basic_Mchi_regression_nodes = this_int_map["basic_Mchi_regression_nodes"];

  // load the  DEM
  LSDRaster topography_raster;
  if (this_bool_map["remove_seas"])
  {
    cout << "I am removing high and low values to get rid of things that should be nodata." << endl;
    LSDRaster start_raster((DATA_DIR+DEM_ID), raster_ext);
    // now get rid of the low and high values
    float lower_threshold = this_float_map["minimum_elevation"];
    float upper_threshold = this_float_map["maximum_elevation"];
    bool belowthresholdisnodata = true;
    LSDRaster Flooded = start_raster.mask_to_nodata_using_threshold(lower_threshold,belowthresholdisnodata);
    belowthresholdisnodata = false;
    topography_raster = Flooded.mask_to_nodata_using_threshold(upper_threshold,belowthresholdisnodata);
  }
  else
  {
    LSDRaster start_raster((DATA_DIR+DEM_ID), raster_ext);
    topography_raster = start_raster;
  }
  cout << "Got the dem: " <<  DATA_DIR+DEM_ID << endl;

  float Resolution = topography_raster.get_DataResolution();
  map<string,string> GRS = topography_raster.get_GeoReferencingStrings();

  float thresh_area_for_chi = float(this_int_map["threshold_pixels_for_chi"])*Resolution*Resolution;

  if(this_bool_map["only_check_parameters"])
  {
    cout << "You set the only_check_parameters flag to true; I have only printed" << endl;
    cout << "the parameters to file and am now exiting." << endl;
    exit(0);
  }

  //============================================================================
  // Start gathering necessary rasters
  //============================================================================
  LSDRaster filled_topography;
  // now get the flow info object
  if ( this_bool_map["raster_is_filled"] )
  {
    cout << "You have chosen to use a filled raster." << endl;
    filled_topography = topography_raster;
  }
  else
  {
    cout << "Let me fill that raster for you, the min slope is: "
         << this_float_map["min_slope_for_fill"] << endl;
    filled_topography = topography_raster.fill(this_float_map["min_slope_for_fill"]);
  }

  if (this_bool_map["print_fill_raster"])
  {
    cout << "Let me print the fill raster for you."  << endl;
    string filled_raster_name = OUT_DIR+OUT_ID+"_Fill";
    filled_topography.write_raster(filled_raster_name,raster_ext);
  }

  if (this_bool_map["write hillshade"])
  {
    cout << "Let me print the hillshade for you. " << endl;
    float hs_azimuth = 315;
    float hs_altitude = 45;
    float hs_z_factor = 1;
    LSDRaster hs_raster = topography_raster.hillshade(hs_altitude,hs_azimuth,hs_z_factor);

    string hs_fname = OUT_DIR+OUT_ID+"_hs";
    hs_raster.write_raster(hs_fname,raster_ext);
  }


  cout << "\t Flow routing..." << endl;
  // get a flow info object
  LSDFlowInfo FlowInfo(boundary_conditions,filled_topography);

  // calculate the flow accumulation
  cout << "\t Calculating flow accumulation (in pixels)..." << endl;
  LSDIndexRaster FlowAcc = FlowInfo.write_NContributingNodes_to_LSDIndexRaster();

  cout << "\t Converting to flow area..." << endl;
  LSDRaster DrainageArea = FlowInfo.write_DrainageArea_to_LSDRaster();

  if (this_bool_map["print_DrainageArea_raster"])
  {
    string DA_raster_name = OUT_DIR+OUT_ID+"_DArea";
    DrainageArea.write_raster(DA_raster_name,raster_ext);
  }

  // calcualte the distance from outlet
  cout << "\t Calculating flow distance..." << endl;
  LSDRaster DistanceFromOutlet = FlowInfo.distance_from_outlet();

  cout << "\t Loading Sources..." << endl;
  cout << "\t Source file is... " << CHeads_file << endl;

  // load the sources
  vector<int> sources;
  if (CHeads_file == "NULL" || CHeads_file == "Null" || CHeads_file == "null")
  {
    cout << endl << endl << endl << "==================================" << endl;
    cout << "The channel head file is null. " << endl;
    cout << "Getting sources from a threshold of "<< threshold_contributing_pixels << " pixels." <<endl;
    sources = FlowInfo.get_sources_index_threshold(FlowAcc, threshold_contributing_pixels);

    cout << "The number of sources is: " << sources.size() << endl;
  }
  else
  {
    cout << "Loading channel heads from the file: " << DATA_DIR+CHeads_file << endl;
    sources = FlowInfo.Ingest_Channel_Heads((DATA_DIR+CHeads_file), "csv",2);
    cout << "\t Got sources!" << endl;
  }

  // now get the junction network
  LSDJunctionNetwork JunctionNetwork(sources, FlowInfo);

  // Print channels and junctions if you want them.
  if( this_bool_map["print_channels_to_csv"])
  {
    string channel_csv_name = OUT_DIR+OUT_ID+"_CN";
    JunctionNetwork.PrintChannelNetworkToCSV(FlowInfo, channel_csv_name);

    // convert to geojson if that is what the user wants
    // It is read more easily by GIS software but has bigger file size
    if ( this_bool_map["convert_csv_to_geojson"])
    {
      string gjson_name = OUT_DIR+OUT_ID+"_CN.geojson";
      LSDSpatialCSVReader thiscsv(OUT_DIR+OUT_ID+"_CN.csv");
      thiscsv.print_data_to_geojson(gjson_name);
    }
  }

  // print junctions
  if( this_bool_map["print_junctions_to_csv"])
  {
    cout << "I am writing the junctions to csv." << endl;
    string channel_csv_name = OUT_DIR+OUT_ID+"_JN.csv";
    JunctionNetwork.print_junctions_to_csv(FlowInfo, channel_csv_name);

    if ( this_bool_map["convert_csv_to_geojson"])
    {
      string gjson_name = OUT_DIR+OUT_ID+"_JN.geojson";
      LSDSpatialCSVReader thiscsv(channel_csv_name);
      thiscsv.print_data_to_geojson(gjson_name);
    }
  }

  // Print sources
  if( this_bool_map["print_sources_to_csv"])
  {
    string sources_csv_name = OUT_DIR+OUT_ID+"_ATsources.csv";

    //write channel_heads to a csv file
    FlowInfo.print_vector_of_nodeindices_to_csv_file_with_latlong(sources, sources_csv_name);

    if ( this_bool_map["convert_csv_to_geojson"])
    {
      string gjson_name = OUT_DIR+OUT_ID+"_ATsources.geojson";
      LSDSpatialCSVReader thiscsv(sources_csv_name);
      thiscsv.print_data_to_geojson(gjson_name);
    }
  }

  if (this_bool_map["print_stream_order_raster"])
  {
    LSDIndexRaster SOArray = JunctionNetwork.StreamOrderArray_to_LSDIndexRaster();
    string SO_raster_name = OUT_DIR+OUT_ID+"_SO";
    SOArray.write_raster(SO_raster_name,raster_ext);
  }
  if (this_bool_map["print_junction_index_raster"])
  {
    LSDIndexRaster JIArray = JunctionNetwork.JunctionIndexArray_to_LSDIndexRaster();
    string JI_raster_name = OUT_DIR+OUT_ID+"_JI";
    JIArray.write_raster(JI_raster_name,raster_ext);
  }

  // need to get base-level nodes , otherwise these catchments will be missed!
  vector< int > BaseLevelJunctions;
  vector< int > BaseLevelJunctions_Initial;
  
  //Check to see if a list of junctions for extraction exists
  if (BaselevelJunctions_file == "NULL" || BaselevelJunctions_file == "Null" || BaselevelJunctions_file == "null" || BaselevelJunctions_file.empty() == true)
  {
    cout << "No junction file. I am going to select basins for you using an algorithm. " << endl;
    // remove basins drainage from edge if that is what the user wants
    if (this_bool_map["find_complete_basins_in_window"])
    {
      cout << "I am going to look for basins in a pixel window that are not influended by nodata." << endl;
      cout << "I am also going to remove any nested basins." << endl;
      BaseLevelJunctions = JunctionNetwork.Prune_Junctions_By_Contributing_Pixel_Window_Remove_Nested_And_Nodata(FlowInfo, filled_topography, FlowAcc, 
                                              this_int_map["minimum_basin_size_pixels"],this_int_map["maximum_basin_size_pixels"]);
    }
    else
    {
      //Get baselevel junction nodes from the whole network
      BaseLevelJunctions_Initial = JunctionNetwork.get_BaseLevelJunctions();
      
      // now prune these by drainage area
      cout << "Removing basins with fewer than " << minimum_basin_size_pixels << " pixels" << endl;
      BaseLevelJunctions = JunctionNetwork.Prune_Junctions_Area(BaseLevelJunctions_Initial,
                                              FlowInfo, FlowAcc, minimum_basin_size_pixels);
      cout << "Now I have " << BaseLevelJunctions.size() << " baselelvel junctions left. " << endl;
      
      if (this_bool_map["find_largest_complete_basins"])
      {
        cout << "I am looking for the largest basin not influenced by nodata within all baselevel nodes." << endl;
        BaseLevelJunctions = JunctionNetwork.Prune_To_Largest_Complete_Basins(BaseLevelJunctions,FlowInfo, filled_topography, FlowAcc);
      }
      else
      {
        if (this_bool_map["test_drainage_boundaries"])     // now check for edge effects
        {
          cout << endl << endl << "I am going to remove basins draining to the edge." << endl;
          BaseLevelJunctions = JunctionNetwork.Prune_Junctions_Edge_Ignore_Outlet_Reach(BaseLevelJunctions,FlowInfo, filled_topography);
          //BaseLevelJunctions = JunctionNetwork.Prune_Junctions_Edge(BaseLevelJunctions,FlowInfo);
        }
      }
    }
  }
  else
  {
    cout << "I am attempting to read junctions from a junction list." << endl;
    //specify junctions to work on from a list file
    string JunctionsFile = DATA_DIR+DEM_ID+"_junctions.list";

    vector<int> JunctionsList;
    ifstream infile(JunctionsFile.c_str());
    if (infile)
    {
      cout << "Junctions File " << JunctionsFile << " exists" << endl;;
      int n;
      while (infile >> n) BaseLevelJunctions_Initial.push_back(n);
    }
    else
    {
      cout << "Fatal Error: Junctions File " << JunctionsFile << " does not exist" << endl;
      exit(EXIT_FAILURE);
    }
  }

  // Now check for larges basin, if that is what you want. 
  if (this_bool_map["only_take_largest_basin"])
  {
    cout << "I am only going to take the largest basin." << endl;
    BaseLevelJunctions = JunctionNetwork.Prune_Junctions_Largest(BaseLevelJunctions, FlowInfo, FlowAcc);
  }

  // Correct number of base level junctions
  int N_BaseLevelJuncs = BaseLevelJunctions.size();
  cout << "The number of basins I will analyse is: " << N_BaseLevelJuncs << endl;
  if (N_BaseLevelJuncs == 0)
  {
    cout << "I am stopping here since I don't have any basins to analyse." << endl;
    exit(EXIT_FAILURE);
  }

  // This is for debugging
  //for (int BN = 0; BN< N_BaseLevelJuncs; BN++)
  //{
  //  cout << "BL junc is: " << BaseLevelJunctions[BN] << " node is: " << JunctionNetwork.get_Node_of_Junction(BaseLevelJunctions[BN]) << endl;
  //  vector<int> UPSN = FlowInfo.get_upslope_nodes(JunctionNetwork.get_Node_of_Junction(BaseLevelJunctions[BN]));
  //  cout << "Pixels for that node are: " << UPSN.size() << endl;
  //}

  //============================================================================
  // THE CHI STUFF STARTS HERE
  // now use a ChiTool object to print the chi tree to csv
  LSDChiTools ChiTool(FlowInfo);

  // calculate chi for the entire DEM
  cout << "Calculating the chi coordinate for A_0: " << A_0 << " and m/n: " << movern << endl;
  LSDRaster chi_coordinate;
  LSDRaster Discharge;
  if(this_bool_map["use_precipitation_raster_for_chi"])
  {

    string Precip_f_name = DATA_DIR+this_string_map["precipitation_fname"];
    cout << "I am loading a precipitation raster. " << Precip_f_name<<".bil" << endl;
    cout << "Note this MUST be the same size as the base DEM or it will crash!" << endl;

    if(this_string_map["precipitation_fname"]=="NULL")
    {
      cout << "You have asked to use a precipitation raster but have not given a name." << endl;
      cout << "Set the name of the raster with the keyword precipitation_fname" << endl;
      exit(EXIT_FAILURE);
    }

    // calculate the discharge
    // note: not discharge yet, need to multiply by cell area
    LSDRaster VolumePrecipitation(Precip_f_name,raster_ext);
    float dx = VolumePrecipitation.get_DataResolution();

    // volume precipitation per time precipitation times the cell areas
    VolumePrecipitation.raster_multiplier(dx*dx);

    // discharge accumulates this precipitation
    Discharge = FlowInfo.upslope_variable_accumulator(VolumePrecipitation);
    chi_coordinate = FlowInfo.get_upslope_chi_from_all_baselevel_nodes(movern,A_0,thresh_area_for_chi,Discharge);

    if(this_bool_map["print_discharge_raster"])
    {
      string Discharge_fname = OUT_DIR+OUT_ID+"_Q";
      Discharge.write_raster(Discharge_fname, raster_ext);
    }
    // Print the chi raster
    if(this_bool_map["print_chi_coordinate_raster"])
    {
      string chi_coord_string = OUT_DIR+OUT_ID+"_chi_coordQ";
      chi_coordinate.write_raster(chi_coord_string,raster_ext);
    }


  }
  else
  {
    chi_coordinate = FlowInfo.get_upslope_chi_from_all_baselevel_nodes(movern,A_0,thresh_area_for_chi);
    // Print the chi raster
    if(this_bool_map["print_chi_coordinate_raster"])
    {
      string chi_coord_string = OUT_DIR+OUT_ID+"_chi_coord";
      chi_coordinate.write_raster(chi_coord_string,raster_ext);
    }
  }

  // This bit prints a chi coordinate raster even if you are using precipitation
  if(this_bool_map["print_chi_coordinate_raster"] && this_bool_map["use_precipitation_raster_for_chi"] && this_bool_map["print_chi_no_discharge"])
  {
    LSDRaster NoDischargeChi = FlowInfo.get_upslope_chi_from_all_baselevel_nodes(movern,A_0,thresh_area_for_chi);
    string chi_coord_string = OUT_DIR+OUT_ID+"_chi_coord";
    NoDischargeChi.write_raster(chi_coord_string,raster_ext);
  }
  //============================================================================



  if (this_bool_map["print_simple_chi_map_to_csv"])
  {
    cout <<"I am printing a simple chi map for you to csv." << endl;

    string chi_csv_fname = OUT_DIR+OUT_ID+"_chi_coord.csv";
    ChiTool.chi_map_to_csv(FlowInfo, chi_csv_fname, chi_coordinate);

    if ( this_bool_map["convert_csv_to_geojson"])
    {
      string gjson_name = OUT_DIR+OUT_ID+"_chi_coord.geojson";
      LSDSpatialCSVReader thiscsv(chi_csv_fname);
      thiscsv.print_data_to_geojson(gjson_name);
    }

  }



  //============================================================================
  // Print basins with chi to csv
  if (this_bool_map["print_simple_chi_map_with_basins_to_csv"])
  {
    cout <<"I am printing a simple chi map with basins for you to csv." << endl;
    LSDIndexRaster basin_raster = ChiTool.get_basin_raster(FlowInfo, JunctionNetwork, BaseLevelJunctions);
    string chi_csv_fname = OUT_DIR+DEM_ID+"_chi_coord_basins.csv";
    ChiTool.chi_map_to_csv(FlowInfo, chi_csv_fname, chi_coordinate,basin_raster);

    if ( this_bool_map["convert_csv_to_geojson"])
    {
      string gjson_name = OUT_DIR+DEM_ID+"_chi_coord_basins.geojson";
      LSDSpatialCSVReader thiscsv(chi_csv_fname);
      thiscsv.print_data_to_geojson(gjson_name);
    }
  }


  // now source and outlet nodes for segmentation and other operations.
  vector<int> source_nodes;
  vector<int> outlet_nodes;
  vector<int> baselevel_node_of_each_basin;
  if (this_bool_map["print_segmented_M_chi_map_to_csv"]
        || this_bool_map["print_basic_M_chi_map_to_csv"]
        || this_bool_map["calculate_MLE_collinearity"]
        || this_bool_map["print_profiles_fxn_movern_csv"]
        || this_bool_map["print_slope_area_data"]
        || this_bool_map["print_source_keys"]
        || this_bool_map["print_baselevel_keys"]
        || this_bool_map["print_basin_raster"])
  {
    cout << "I am getting the source and outlet nodes for the overlapping channels" << endl;
    cout << "The n_nodes to visit are: " << n_nodes_to_visit << endl;
    
    //Check to see if working with a specified list of baselevel junctions
    if (this_string_map["BaselevelJunctions_file"] == "NULL" 
        || this_string_map["BaselevelJunctions_file"] == "Null" 
        || this_string_map["BaselevelJunctions_file"] == "null")
    {
      cout << "I am not working with a BaselevelJunctions_file." << endl;
      JunctionNetwork.get_overlapping_channels(FlowInfo, BaseLevelJunctions, DistanceFromOutlet,
                                    source_nodes,outlet_nodes,baselevel_node_of_each_basin,n_nodes_to_visit);
    }
    else
    {
      JunctionNetwork.get_overlapping_channels_to_downstream_outlets(FlowInfo, BaseLevelJunctions, DistanceFromOutlet,
                                    source_nodes,outlet_nodes,baselevel_node_of_each_basin,n_nodes_to_visit);
    }
  }

  if (this_bool_map["print_segmented_M_chi_map_to_csv"])
  {
    cout << "I am calculating the segmented channels" << endl;
    if (source_nodes.size() == 0)
    {
      cout << "I don't seem to have any source nodes!" << endl;
    }

    // check to see if we want segments. If that is the case, the
    // skip and iterations default to 0 and 1
    if (this_bool_map["print_segments"])
    {
      n_iterations = 1;
      skip = 0;
      ChiTool.chi_map_automator(FlowInfo, source_nodes, outlet_nodes, baselevel_node_of_each_basin,
                            filled_topography, DistanceFromOutlet,
                            DrainageArea, chi_coordinate, target_nodes,
                            n_iterations, skip, minimum_segment_length, sigma);
      ChiTool.segment_counter(FlowInfo);
    }
    else
    {
      ChiTool.chi_map_automator(FlowInfo, source_nodes, outlet_nodes, baselevel_node_of_each_basin,
                            filled_topography, DistanceFromOutlet,
                            DrainageArea, chi_coordinate, target_nodes,
                            n_iterations, skip, minimum_segment_length, sigma);
    }

    string csv_full_fname = OUT_DIR+OUT_ID+"_MChiSegmented.csv";
    cout << "Let me print all the data for you into a csv file called " << csv_full_fname << endl;
    ChiTool.print_data_maps_to_file_full(FlowInfo, csv_full_fname);
    cout << "That is your file printed!" << endl;

    if ( this_bool_map["convert_csv_to_geojson"])
    {
      cout << "Now let me print your chi network to a geojson" << endl;
      string gjson_name = OUT_DIR+OUT_ID+"_MChiSegmented.geojson";
      LSDSpatialCSVReader thiscsv(csv_full_fname);
      thiscsv.print_data_to_geojson(gjson_name);
    }
  }

  //============================================================================
  // CHECK CHI IN THE CHITOOL OBJECT
  // This is really only for debugging
  //============================================================================
  if(this_bool_map["check_chi_maps"])
  {
    float thresh_area_for_chi = 0;
    LSDChiTools ChiTool_chi_checker(FlowInfo);
    ChiTool_chi_checker.chi_map_automator_chi_only(FlowInfo, source_nodes, outlet_nodes, baselevel_node_of_each_basin,
                            filled_topography, DistanceFromOutlet,
                            DrainageArea, chi_coordinate);

    // first the logic if you are using a precipitation raster
    if(this_bool_map["use_precipitation_raster_for_chi"])
    {
      string chiQ_data_maps_string = OUT_DIR+OUT_ID+"_checkchiQ.csv";
      ChiTool_chi_checker.print_chi_data_map_to_csv(FlowInfo, chiQ_data_maps_string);

      if ( this_bool_map["convert_csv_to_geojson"])
      {
        string gjson_name = OUT_DIR+OUT_ID+"_checkchiQ.geojson";
        LSDSpatialCSVReader thiscsv(chiQ_data_maps_string);
        thiscsv.print_data_to_geojson(gjson_name);
      }

      // now get the chi coordinate without the discharge
      LSDRaster chi_noQ = FlowInfo.get_upslope_chi_from_all_baselevel_nodes(movern,A_0,thresh_area_for_chi);
      ChiTool_chi_checker.chi_map_automator_chi_only(FlowInfo, source_nodes, outlet_nodes, baselevel_node_of_each_basin,
                            filled_topography, DistanceFromOutlet,
                            DrainageArea, chi_noQ);
      string chi_data_maps_string = OUT_DIR+OUT_ID+"_checkchi.csv";
      ChiTool_chi_checker.print_chi_data_map_to_csv(FlowInfo, chi_data_maps_string);

      if ( this_bool_map["convert_csv_to_geojson"])
      {
        string gjson_name = OUT_DIR+OUT_ID+"_checkchi.geojson";
        LSDSpatialCSVReader thiscsv(chi_data_maps_string);
        thiscsv.print_data_to_geojson(gjson_name);
      }
    }
    else
    {
      string chi_data_maps_string = OUT_DIR+OUT_ID+"_checkchi.csv";
      ChiTool_chi_checker.print_chi_data_map_to_csv(FlowInfo, chi_data_maps_string);

      if ( this_bool_map["convert_csv_to_geojson"])
      {
        string gjson_name = OUT_DIR+OUT_ID+"_checkchi.geojson";
        LSDSpatialCSVReader thiscsv(chi_data_maps_string);
        thiscsv.print_data_to_geojson(gjson_name);
      }
    }
  }
  //============================================================================

  if (this_bool_map["calculate_MLE_collinearity"])
  {

    cout << "I am testing the collinearity for you. " << endl;
    // Lets make a new chi tool: this won't be segmented since we only
    // need it for m/n
    LSDChiTools ChiTool_movern(FlowInfo);
    ChiTool_movern.chi_map_automator_chi_only(FlowInfo, source_nodes, outlet_nodes, baselevel_node_of_each_basin,
                            filled_topography, DistanceFromOutlet,
                            DrainageArea, chi_coordinate);

    // test the basin collinearity test
    //int baselevel_key = 1;
    vector<int> reference_source;
    vector<int> test_source;
    vector<float> MLE_values;
    vector<float> RMSE_values;
    //bool only_use_mainstem_as_reference = true;

    if(this_bool_map["use_precipitation_raster_for_chi"])
    {
      cout << "Using a discharge raster to check collinearity." << endl;
      string movern_name = OUT_DIR+OUT_ID+"_movernstatsQ";
      ChiTool_movern.calculate_goodness_of_fit_collinearity_fxn_movern_with_discharge(FlowInfo,
                      JunctionNetwork, this_float_map["start_movern"], this_float_map["delta_movern"],
                      this_int_map["n_movern"],
                      this_bool_map["only_use_mainstem_as_reference"],
                      movern_name, Discharge);
    }
    else
    {
      string movern_name = OUT_DIR+OUT_ID+"_movernstats";
      ChiTool_movern.calculate_goodness_of_fit_collinearity_fxn_movern(FlowInfo, JunctionNetwork,
                      this_float_map["start_movern"], this_float_map["delta_movern"],
                      this_int_map["n_movern"],
                      this_bool_map["only_use_mainstem_as_reference"],
                      movern_name);
    }
  }

  if(this_bool_map["print_profiles_fxn_movern_csv"] )
  {
    cout << endl << "Let me loop through m/n values and print the profiles to a single csv." << endl;
    // Lets make a new chi tool: this won't be segmented since we only
    // need it for m/n
    LSDChiTools ChiTool_movern(FlowInfo);

    cout << "Running automator" << endl;
    // we always need to run the automator first
    ChiTool_movern.chi_map_automator_chi_only(FlowInfo, source_nodes, outlet_nodes, baselevel_node_of_each_basin,
                            filled_topography, DistanceFromOutlet,
                            DrainageArea, chi_coordinate);


    cout << "Looping m over n" << endl;
    // now loop through m/n values, printing them all to the csv file


    if(this_bool_map["use_precipitation_raster_for_chi"])
    {
      string movern_name = OUT_DIR+OUT_ID+"_movernQ.csv";
      cout << "Using a discharge raster to calculate m over n." << endl;
      ChiTool_movern.print_profiles_as_fxn_movern_with_discharge(FlowInfo, movern_name,
                                  this_float_map["start_movern"],
                                  this_float_map["delta_movern"],
                                  this_int_map["n_movern"],
                                  Discharge);
    }
    else
    {
      string movern_name = OUT_DIR+OUT_ID+"_movern.csv";
      ChiTool_movern.print_profiles_as_fxn_movern(FlowInfo, movern_name,
                                  this_float_map["start_movern"],
                                  this_float_map["delta_movern"],
                                  this_int_map["n_movern"]);
    }
  }

  if (this_bool_map["print_slope_area_data"])
  {
    cout << "I am going to calculate slope-area data for you. " << endl;
    LSDChiTools ChiTool_SA(FlowInfo);
    ChiTool_SA.chi_map_automator_chi_only(FlowInfo, source_nodes, outlet_nodes, baselevel_node_of_each_basin,
                            filled_topography, DistanceFromOutlet,
                            DrainageArea, chi_coordinate);

    float vertical_interval = this_float_map["SA_vertical_interval"];
    string filename_SA = OUT_DIR+OUT_ID+"_SAvertical.csv";
    string filename_binned = OUT_DIR+OUT_ID+"_SAbinned.csv";

    vector<int> SA_midpoint_nodes;
    vector<float> SA_slopes;
    ChiTool_SA.get_slope_area_data(FlowInfo, vertical_interval,
                                   SA_midpoint_nodes,SA_slopes);

    cout << "Printing raw S-A data." << endl;
    ChiTool_SA.print_slope_area_data_to_csv(FlowInfo, SA_midpoint_nodes, SA_slopes, filename_SA);

    cout << "Printing binned S-A data." << endl;
    ChiTool_SA.bin_slope_area_data(FlowInfo, SA_midpoint_nodes, SA_slopes, this_float_map["log_A_bin_width"],filename_binned);
  }

  if (this_bool_map["print_basic_M_chi_map_to_csv"])
  {
    // Note that this uses the chi coordinate derived from early in this code
    // and will use the discharge-based raster if that has been called.
    LSDChiTools ChiTool2(FlowInfo);
    ChiTool2.chi_map_automator_rudimentary(FlowInfo, source_nodes,outlet_nodes, baselevel_node_of_each_basin,
                                    filled_topography, DistanceFromOutlet, DrainageArea,
                                    chi_coordinate, basic_Mchi_regression_nodes);
    string csv_full_fname = OUT_DIR+OUT_ID+"_MChiBasic.csv";
    ChiTool2.print_data_maps_to_file_full(FlowInfo, csv_full_fname);

    if ( this_bool_map["convert_csv_to_geojson"])
    {
      string gjson_name = OUT_DIR+OUT_ID+"_MChiBasic.geojson";
      LSDSpatialCSVReader thiscsv(csv_full_fname);
      thiscsv.print_data_to_geojson(gjson_name);
    }
  }


  if (this_bool_map["print_source_keys"]
        || this_bool_map["print_baselevel_keys"])
  {
    cout << "I am going to print the source and baselevel keys for you. " << endl;
    LSDChiTools ChiTool_keys(FlowInfo);
    ChiTool_keys.chi_map_automator_chi_only(FlowInfo, source_nodes, outlet_nodes, baselevel_node_of_each_basin,
                            filled_topography, DistanceFromOutlet,
                            DrainageArea, chi_coordinate);

    // These print the source and baselelvel keys if wanted
    if (this_bool_map["print_source_keys"])
    {
      string sources_keys_name = OUT_DIR+OUT_ID+"_SourceKeys.csv";
      ChiTool_keys.print_source_keys(FlowInfo, sources_keys_name);

      if ( this_bool_map["convert_csv_to_geojson"])
      {
        string gjson_name = OUT_DIR+OUT_ID+"_SourceKeys.geojson";
        LSDSpatialCSVReader thiscsv(sources_keys_name);
        thiscsv.print_data_to_geojson(gjson_name);
      }

    }
    if (this_bool_map["print_baselevel_keys"])
    {
      string baselevel_keys_name = OUT_DIR+OUT_ID+"_BaselevelKeys.csv";
      ChiTool_keys.print_baselevel_keys(FlowInfo, JunctionNetwork, baselevel_keys_name);

      if ( this_bool_map["convert_csv_to_geojson"])
      {
        string gjson_name = OUT_DIR+OUT_ID+"_BaselevelKeys.geojson";
        LSDSpatialCSVReader thiscsv(baselevel_keys_name);
        thiscsv.print_data_to_geojson(gjson_name);
      }
    }
  }

  //============================================================================
  // Print a basin raster if you want it.
  if(this_bool_map["print_basin_raster"])
  {
    cout << "I am going to print the basins for you. " << endl;
    LSDChiTools ChiTool_basins(FlowInfo);
    ChiTool_basins.chi_map_automator_chi_only(FlowInfo, source_nodes, outlet_nodes, baselevel_node_of_each_basin,
                            filled_topography, DistanceFromOutlet,
                            DrainageArea, chi_coordinate);
    string basin_raster_prefix = OUT_DIR+OUT_ID;
    ChiTool_basins.print_basins(FlowInfo, JunctionNetwork, BaseLevelJunctions, basin_raster_prefix);
  }






  if(this_bool_map["ksn_knickpoint_analysis"])
  {
    n_iterations = 1;
    skip = 0;
    ChiTool.chi_map_automator(FlowInfo, source_nodes, outlet_nodes, baselevel_node_of_each_basin,
                          filled_topography, DistanceFromOutlet,
                          DrainageArea, chi_coordinate, target_nodes,
                          n_iterations, skip, minimum_segment_length, sigma);
    ChiTool.segment_counter(FlowInfo);
    ChiTool.ksn_knickpoint_detection(FlowInfo);
    string csv_full_fname_knockpoint = OUT_DIR+OUT_ID+"_KsnKn.csv";
    ChiTool.print_knickpoint_to_csv(FlowInfo,csv_full_fname_knockpoint);
  }


}
