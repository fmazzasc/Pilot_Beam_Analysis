o2-its-reco-workflow --trackerCA --tracking-mode async --configKeyValues 'ITSVertexerParam.phiCut=0.5; ITSVertexerParam.clusterContributorsCut=3; ITSVertexerParam.tanLambdaCut=0.2' -b
o2-primary-vertexing-workflow --vertex-track-matching-sources ITS --vertexing-sources ITS -b
o2-secondary-vertexing-workflow --vertexing-sources ITS --shm-segment-size 10000000000 -b
