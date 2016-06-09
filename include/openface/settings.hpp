#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#define FACE_SIZE_CONSTRAINT 96
#define NEURAL_NETWORK "resources/nn4.v2.t7"
#define FACE_SHAPE "resources/shape_predictor_68_face_landmarks.dat"
#define FORWARD_DEFINITION "src/openface/forward_nn.lua"

static const cv::Point2f MINMAX_TEMPLATE[] = {
    cv::Point2f( 0.          ,0.17856914),
    cv::Point2f( 0.00412831  ,0.31259227),
    cv::Point2f( 0.0196793   ,0.44770938),
    cv::Point2f( 0.04809872  ,0.5800727 ),
    cv::Point2f( 0.10028344  ,0.70349526),
    cv::Point2f( 0.17999782  ,0.81208664),
    cv::Point2f( 0.27627307  ,0.90467805),
    cv::Point2f( 0.38463727  ,0.98006284),
    cv::Point2f( 0.50735611  ,1.        ),
    cv::Point2f( 0.63014114  ,0.97611183),
    cv::Point2f( 0.73867768  ,0.89921385),
    cv::Point2f( 0.83547473  ,0.80513287),
    cv::Point2f( 0.91434467  ,0.69456232),
    cv::Point2f( 0.9643504   ,0.56826204),
    cv::Point2f( 0.98870581  ,0.43244401),
    cv::Point2f( 0.99931228  ,0.29529294),
    cv::Point2f( 1.          ,0.15909716),
    cv::Point2f( 0.09485531  ,0.07603313),
    cv::Point2f( 0.15534875  ,0.02492465),
    cv::Point2f( 0.2377474   ,0.01139098),
    cv::Point2f( 0.32313403  ,0.02415778),
    cv::Point2f( 0.40366989  ,0.05780071),
    cv::Point2f( 0.56864655  ,0.0521157 ),
    cv::Point2f( 0.65128165  ,0.01543965),
    cv::Point2f( 0.73796082  ,0.        ),
    cv::Point2f( 0.82290924  ,0.01191543),
    cv::Point2f( 0.88739765  ,0.06025707),
    cv::Point2f( 0.48893312  ,0.15513189),
    cv::Point2f( 0.48991537  ,0.24343018),
    cv::Point2f( 0.49092147  ,0.33176517),
    cv::Point2f( 0.49209353  ,0.42210701),
    cv::Point2f( 0.39739901  ,0.48004663),
    cv::Point2f( 0.4442625   ,0.49906778),
    cv::Point2f( 0.49495089  ,0.51444137),
    cv::Point2f( 0.54558265  ,0.49682876),
    cv::Point2f( 0.59175086  ,0.47722608),
    cv::Point2f( 0.194157    ,0.16926692),
    cv::Point2f( 0.24600308  ,0.13693026),
    cv::Point2f( 0.31000495  ,0.13735634),
    cv::Point2f( 0.36378494  ,0.17794687),
    cv::Point2f( 0.3063696   ,0.19082251),
    cv::Point2f( 0.24390514  ,0.19138186),
    cv::Point2f( 0.61896318  ,0.17277813),
    cv::Point2f( 0.67249435  ,0.12988105),
    cv::Point2f( 0.73628569  ,0.1279085 ),
    cv::Point2f( 0.78885913  ,0.15817115),
    cv::Point2f( 0.74115133  ,0.18155812),
    cv::Point2f( 0.67913717  ,0.18370388),
    cv::Point2f( 0.30711025  ,0.6418497 ),
    cv::Point2f( 0.3759703   ,0.61095947),
    cv::Point2f( 0.44670257  ,0.59705079),
    cv::Point2f( 0.49721557  ,0.60872644),
    cv::Point2f( 0.5500201   ,0.5954327 ),
    cv::Point2f( 0.62330163  ,0.60709113),
    cv::Point2f( 0.69541407  ,0.63414288),
    cv::Point2f( 0.62806797  ,0.70906836),
    cv::Point2f( 0.5573954   ,0.74344712),
    cv::Point2f( 0.50020397  ,0.75058442),
    cv::Point2f( 0.44528747  ,0.74580276),
    cv::Point2f( 0.37508208  ,0.71454251),
    cv::Point2f( 0.33728781  ,0.64616466),
    cv::Point2f( 0.44701463  ,0.64064664),
    cv::Point2f( 0.49795204  ,0.64496332),
    cv::Point2f( 0.55139428  ,0.63859367),
    cv::Point2f( 0.66502279  ,0.63955915),
    cv::Point2f( 0.55305558  ,0.67647934),
    cv::Point2f( 0.49864811  ,0.68417645),
    cv::Point2f( 0.44657204  ,0.67860472)
};

static const cv::Point2f OUTER_EYES_AND_NOSE[] = {
    96*MINMAX_TEMPLATE[36],
    96*MINMAX_TEMPLATE[45],
    96*MINMAX_TEMPLATE[33]
};


#endif
