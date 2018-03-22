set(Sources
        reporting/console_reporter.cc
        reporting/json_reporter.cc
        reporting/util.cc
        reporting/abstract_polling_reporter.cc
        reporting/collectd_reporter.cc
        timer_context.cc
        metric_processor.cc
        counter.cc
        metric_name.cc
        histogram.cc
        timer.cc
        metrics_registry.cc
        meter.cc
        stats/snapshot.cc
        stats/ewma.cc
        stats/exp_decay_sample.cc
        stats/uniform_sample.cc
        )


set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/medida/types.h
        ${VIXAL_INCLUDE_DIR}/medida/timer_context.h
        ${VIXAL_INCLUDE_DIR}/medida/timer.h
        ${VIXAL_INCLUDE_DIR}/medida/counter.h
        ${VIXAL_INCLUDE_DIR}/medida/histogram.h
        ${VIXAL_INCLUDE_DIR}/medida/medida.h
        ${VIXAL_INCLUDE_DIR}/medida/meter.h
        ${VIXAL_INCLUDE_DIR}/medida/metered_interface.h
        ${VIXAL_INCLUDE_DIR}/medida/metric_interface.h
        ${VIXAL_INCLUDE_DIR}/medida/metric_processor.h
        ${VIXAL_INCLUDE_DIR}/medida/metrics_registry.h
        ${VIXAL_INCLUDE_DIR}/medida/metric_name.h
        ${VIXAL_INCLUDE_DIR}/medida/sampling_interface.h
        ${VIXAL_INCLUDE_DIR}/medida/summarizable_interface.h
        ${VIXAL_INCLUDE_DIR}/medida/reporting/abstract_polling_reporter.h
        ${VIXAL_INCLUDE_DIR}/medida/reporting/collectd_reporter.h
        ${VIXAL_INCLUDE_DIR}/medida/reporting/collectd_reporter.h
        ${VIXAL_INCLUDE_DIR}/medida/reporting/console_reporter.h
        ${VIXAL_INCLUDE_DIR}/medida/reporting/json_reporter.h
        ${VIXAL_INCLUDE_DIR}/medida/reporting/util.h
        ${VIXAL_INCLUDE_DIR}/medida/stats/ewma.h
        ${VIXAL_INCLUDE_DIR}/medida/stats/snapshot.h
        ${VIXAL_INCLUDE_DIR}/medida/stats/sample.h
        ${VIXAL_INCLUDE_DIR}/medida/stats/uniform_sample.h
        ${VIXAL_INCLUDE_DIR}/medida/stats/exp_decay_sample.h
        )

add_library(medida SHARED ${Sources} ${PublicHeaders})


# Says how and where to install software
# Targets:
#   * <prefix>/lib/<libraries>
#   * header location after install: <prefix>/include/<project>/*.h
#   * headers can be included by C++ code `#<project>/Bar.hpp>`
install(TARGETS ${LIBRARY_NAME} ${APPLICATION_NAME}
        EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
        ARCHIVE DESTINATION lib COMPONENT libs   # static lib
        RUNTIME DESTINATION bin COMPONENT libs   # binaries
        LIBRARY DESTINATION lib COMPONENT libs   # shared lib
        FRAMEWORK DESTINATION bin COMPONENT libs # for mac
        PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers for mac (note the different component -> different package)
        INCLUDES DESTINATION include             # headers
        )
