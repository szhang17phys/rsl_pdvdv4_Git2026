#Based on CMakeLists.txt in dualphase folder---
#Modified by Shu, 20230914---

#CRTGenPdvd is CRTGenerator specifically for protoDUNE_VD,
#electrons drift along x axis---

cet_build_plugin(
           CRTGenPdvd art::module
           nusimdata::SimulationBase
           cetlib::cetlib
           cetlib_except::cetlib_except
           fhiclcpp::fhiclcpp
           CLHEP
           larcorealg_Geometry
           larcoreobj_SummaryData
           larsim_EventGenerator
           nurandom::RandomUtils_NuRandomService_service
           ART_FRAMEWORK_SERVICES_REGISTRY
           ART_ROOT_IO_TFILESERVICE_SERVICE
           ART_ROOT_IO_TFILE_SUPPORT
           CLHEP
           messagefacility::MF_MessageLogger
  	       ROOT_BASIC_LIB_LIST
           ROOT_GEOM
           ROOT_XMLIO
           ROOT_GDML
)

cet_build_plugin(
           CRTTrajectoryCut art::module
           nusimdata::SimulationBase
           cetlib::cetlib
           cetlib_except::cetlib_except
           fhiclcpp::fhiclcpp
           CLHEP
           larcorealg_Geometry
           larcoreobj_SummaryData
           larsim_EventGenerator
           nurandom::RandomUtils_NuRandomService_service
           ART_FRAMEWORK_SERVICES_REGISTRY
           ART_ROOT_IO_TFILESERVICE_SERVICE
           ART_ROOT_IO_TFILE_SUPPORT
           CLHEP
           messagefacility::MF_MessageLogger
  	       ROOT_BASIC_LIB_LIST
           ROOT_GEOM
           ROOT_XMLIO
           ROOT_GDML
)





install_headers()
install_fhicl()
install_source()
install_scripts()


#add_subdirectory(test)


