                        <div class="row">
                            <div class="col-xl-3 col-md-6 mb-4">
                                <!-- Dashboard info widget 1-->
                                <div class="card border-start-lg border-start-primary h-100">
                                    <div class="card-body">
                                        <div class="d-flex align-items-center">
                                            <div class="flex-grow-1">
                                                <div class="small fw-bold text-primary mb-1">Docks (Active)</div>
                                                <div class="h5"><?php echo $kpi_dock_count ?></div>

                                            </div>
                                            <div class="ms-2"><i class="fas fa-sync fa-2x text-gray-200"></i></div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class="col-xl-3 col-md-6 mb-4">
                                <!-- Dashboard info widget 2-->
                                <div class="card border-start-lg border-start-secondary h-100">
                                    <div class="card-body">
                                        <div class="d-flex align-items-center">
                                            <div class="flex-grow-1">
                                                <div class="small fw-bold text-secondary mb-1">Received</div>
                                                <div class="h5"><?php echo $kpi_total_messages ?></div>

                                            </div>
                                            <div class="ms-2"><i class="fas fa-inbox fa-2x text-gray-200"></i></div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class="col-xl-3 col-md-6 mb-4">
                                <!-- Dashboard info widget 3-->
                                <div class="card border-start-lg border-start-success h-100">
                                    <div class="card-body">
                                        <div class="d-flex align-items-center">
                                            <div class="flex-grow-1">
                                                <div class="small fw-bold text-success mb-1">Sent</div>
                                                <div class="h5"><?php echo $kpi_sent_messages ?></div>

                                            </div>
                                            <div class="ms-2"><i class="fas fa-microphone fa-2x text-gray-200"></i></div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class="col-xl-3 col-md-6 mb-4">
                                <!-- Dashboard info widget 4-->
                                <div class="card border-start-lg border-start-info h-100">
                                    <div class="card-body">
                                        <div class="d-flex align-items-center">
                                            <div class="flex-grow-1">
                                                <div class="small fw-bold text-info mb-1">Retransmission Rate</div>
                                                <div class="h5"><?php echo $kpi_retransmission_rate . "%" ?></div>

                                            </div>
                                            <div class="ms-2"><i class="fas fa-percentage fa-2x text-gray-200"></i></div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>