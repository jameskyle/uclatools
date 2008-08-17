INCLUDE_DIRECTORIES(${UCLATOOLS_SOURCE_DIR}/ImgLib)
SET(TARGETS Analyze2Genesis Averager CC_gr dcm2img equalize EditAnalyzeHeader)
SET(TARGETS ${TARGETS} EditNSlices GEFunc2Analyze gen2bshort Generic2bshort)
SET(TARGETS ${TARGETS} ImageInfo Mathalyze mult2stack npix P2P ReOrderImages)
SET(TARGETS ${TARGETS} SwapBFloat SignaRenum SynthSeries T2Map TimeVolumeSwap)
SET(TARGETS ${TARGETS} Genesis2Analyze Outlier variance)
ADD_LIBRARY(out_lier STATIC Outlier.c)
SET(LIBS ImgLib)

FOREACH(TRGT ${TARGETS})
  ADD_EXECUTABLE(${TRGT} ${TRGT}.c)
  IF("${TRGT}" STREQUAL EditNSlices)
    TARGET_LINK_LIBRARIES(${TRGT} out_lier ${LIBS} )
  ELSE("${TRGT}" STREQUAL EditNSlices)
    TARGET_LINK_LIBRARIES(${TRGT} ${LIBS})
  ENDIF("${TRGT}" STREQUAL EditNSlices)
  
ENDFOREACH(TRGT)


INSTALL(TARGETS ${TARGETS} RUNTIME DESTINATION bin)
