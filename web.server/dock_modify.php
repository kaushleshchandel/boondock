<?php
include "_config.php";
include '_fx.php';

try {
    $pdo = new PDO("mysql:host=$host;dbname=$dbname", $user, $password);
    // execute the stored procedure
    $sql = 'CALL get_user_docks(' . $_SESSION["id"] . ')';

    // call the stored procedure
    $q = $pdo->query($sql);
    $q->setFetchMode(PDO::FETCH_ASSOC);
} catch (PDOException $e) {
    die("Error occurred:" . $e->getMessage());
}

?>

<!DOCTYPE html>
<html lang="en">

<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Boondock Echo 1.0</title>

    <!-- Custom fonts for this template -->
    <link href="vendor/fontawesome-free/css/all.min.css" rel="stylesheet" type="text/css">
    <link href="https://fonts.googleapis.com/css?family=Nunito:200,200i,300,300i,400,400i,600,600i,700,700i,800,800i,900,900i" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="css/sb-admin-2.min.css" rel="stylesheet">

    <!-- Custom styles for this page -->
    <link href="vendor/datatables/dataTables.bootstrap4.min.css" rel="stylesheet">

</head>

<body id="page-top">

    <!-- Page Wrapper -->
    <div id="wrapper">

        <!-- Sidebar -->
        <?php include '_nav.php' ?>
        <!-- End of Sidebar -->

        <!-- Content Wrapper -->
        <div id="content-wrapper" class="d-flex flex-column">

            <!-- Main Content -->
            <div id="content">

                <!-- Topbar -->
                <?php include '_top.php' ?>
                <!-- End of Topbar -->

                <!-- Begin Page Content -->


                <!-- Dock details content -->
                <div class="container-fluid">
                    <!-- DataTales Example -->
                    <div class="card shadow mb-4">
                        <div class="card-header py-3">
                            <h6 class="m-0 font-weight-bold text-primary">Boon Dock - Details</h6>
                        </div>
                        <div class="card mb-4">

                            <div class="card-body">
                                <form>
                                    <!-- Form Row-->

                                    <div class="row gx-3 mb-3">
                                        <!-- Form Group (BoonDock Id)-->
                                        <div class="col-md-4">

                                            <div class="d-flex align-items-center">
                                                <div class="ms-3">
                                                    <div class="small text-muted">Device ID</div>
                                                    <div class="fs-4 text-dark fw-500">123</div>
                                                </div>
                                            </div>

                                        </div>

                                        <!-- Form Group (MAC Address)-->
                                        <div class="col-md-4">

                                            <div class="d-flex align-items-center">
                                                <div class="ms-3">
                                                    <div class="small text-muted">MAC Address</div>
                                                    <div class="fs-4 text-dark fw-500">AABBCCDDEEFF</div>
                                                </div>
                                            </div>

                                        </div>
                                        
                                    </div>


                                    <div class="row gx-3 mb-3">
                                        <!-- Form Group (Created)-->
                                        <div class="col-md-4">

                                            <div class="d-flex align-items-center">
                                                <div class="ms-3">
                                                    <div class="small text-muted">Created</div>
                                                    <div class="fs-4 text-dark fw-500">Aug 18, 2022 @ 10:21 PM</div>
                                                </div>
                                            </div>

                                        </div>

                                        <!-- Form Group (Last seen)-->
                                        <div class="col-md-4">

                                            <div class="d-flex align-items-center">
                                                <div class="ms-3">
                                                    <div class="small text-muted">Last seen</div>
                                                    <div class="fs-4 text-dark fw-500">5 minutes ago</div>
                                                </div>
                                            </div>

                                        </div>

                                        <!-- Form Group (BoonDock Id)-->
                                        <div class="col-md-4">

                                            <div class="d-flex align-items-center">
                                                <div class="ms-3">
                                                    <div class="small text-muted">Online</div>
                                                    <div class="fs-4 text-dark fw-500">YES</div>
                                                </div>
                                            </div>

                                        </div>

                                    </div>
                                   
    
                                    <div class="row gx-3 mb-3">
                                        <!-- Form Group (Boondock Name)-->
                                        <div class="mb-2 col-md-4">
                                            <label class="small mb-1" for="inputBoondockName">Name</label>
                                            <input class="form-control" id="inputBoondockName" type="email" placeholder="Name your Boondock Echo" value="Baofeng UV-5R - Chicago Weather" />
                                        </div>

                                        <!-- Form Group (Name)-->
                                        <div class="col-md-2">
                                            <label class="small mb-1" for="inputStation">Station</label>
                                            <input class="form-control" id="inputStation" type="text" placeholder="Enter the Station name. e.g. Fire" value="Fire" />
                                        </div>
                                        <!-- Form Group (last name)-->
                                        <div class="col-md-2">
                                            <label class="small mb-1" for="inputFrequency">Frequency</label>
                                            <input class="form-control" id="inputFrequency" type="text" placeholder="Enter Station Frequency" value="125.230" />
                                        </div>
                                    </div>

                                    <button class="btn btn-info" type="button">Identify</button>


                                    <hr>
                                    </hr>

                                    
                                    <label class="large mb-1">Boondock Configuration</label>
                                    <div class="row gx-3 mb-3">

                                        <!-- Form Group (Name)-->
                                        <div class="col-md-2">
                                            <label class="small mb-1" for="inputStation">Silence detection (milliSeconds)</label>
                                            <input class="form-control" id="inputStation" type="text" placeholder="Enter the Station name. e.g. Fire" value="2000" />
                                        </div>
                                        <!-- Form Group (last name)-->
                                        <div class="col-md-2">
                                            <label class="small mb-1" for="inputFrequency">Maximum Recording (Seconds)</label>
                                            <input class="form-control" id="inputFrequency" type="text" placeholder="Enter Station Frequency" value="30" />
                                        </div>

                                        <div class="col-md-2">
                                            <label class="small mb-1" for="inputStation">Audio Threshold</label>
                                            <input class="form-control" id="inputStation" type="text" placeholder="Enter the Station name. e.g. Fire" value="50" />
                                        </div>
                                        <!-- Form Group (last name)-->
                                        <div class="col-md-2">
                                            <label class="small mb-1" for="inputFrequency">Amplify audio</label>
                                            <input class="form-control" id="inputFrequency" type="text" placeholder="Enter Station Frequency" value="50" />
                                        </div>
                                    </div>

                                    <!-- Form Group (Group Selection Checkboxes)-->
                                    <div class="mb-3">
            

                                        <div class="form-check">
                                            <input class="form-check-input" id="groupSales" type="checkbox" value="" checked />
                                            <label class="form-check-label" for="groupSales">RX enabled</label>
                                        </div>
                                        <div class="form-check">
                                            <input class="form-check-input" id="groupDevs" type="checkbox" value="" checked />
                                            <label class="form-check-label" for="groupDevs">TX enabled</label>
                                        </div>
                                        <div class="form-check">
                                            <input class="form-check-input" id="groupMarketing" type="checkbox" value="" />
                                            <label class="form-check-label" for="groupMarketing">Play audio on speaker</label>
                                        </div>
                                    </div>

                                    <!-- Submit button-->
                                    <button class="btn btn-primary" type="button">Save changes</button>
                                    <button class="btn btn-secondary" type="button">Cancel</button>
                                </form>

                            </div>


                        </div>

                    </div>
                    <!-- /.Dock details -->

                </div>
                <!-- End of Main Content -->

                <!-- Footer -->
                <?php include '_footer.php' ?>;
                <!-- End of Footer -->

            </div>
            <!-- End of Content Wrapper -->

        </div>
        <!-- End of Page Wrapper -->

        <!-- Scroll to Top Button-->
        <a class="scroll-to-top rounded" href="#page-top">
            <i class="fas fa-angle-up"></i>
        </a>

        <!-- Logout Modal-->
        <?php include '_logout.php' ?>;
        <?php include '_bootstrap.php' ?>;


</body>

</html>