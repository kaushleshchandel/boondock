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
                <div class="container-fluid">

                    <!-- Page Heading -->
                    <!-- DataTales Example -->
                    <div class="card shadow mb-4">

                        <div class="card-header py-3">
                            <h6 class="m-0 font-weight-bold text-primary">My Boondock Echos</h6>
                        </div>
                        <div class="card-body">
                            <div class="table-responsive">
                                <table class="table table-bordered" id="dataTable" width="100%" cellspacing="0">
                                    <thead>
                                        <tr>
                                            <th>ID</th>
                                            <th>MAC</th>
                                            <th>Name</th>
                                            <th>Station</th>
                                            <th>Frequency</th>
                                            <th>RX</th>
                                            <th>TX</th>
                                            <th>Received</th>
                                            <th>Sent</th>
                                            <th>Last seen</th>
                                            <th></th>


                                        </tr>
                                    </thead>

                                    <tbody>

                                        <?php while ($r = $q->fetch()) : ?>
                                            <tr>
                                                <td><?php echo $r['id'] ?></td>
                                                <td><?php echo $r['mac'] ?></i></td>
                                                <td><?php echo $r['name'] ?></td>
                                                <td><?php echo $r['station'] ?></td>
                                                <td><?php echo $r['frequency'] ?></td>
                                                <td><?php echo $r['rx_enabled'] ?></td>
                                                <td><?php echo $r['tx_enabled'] ?></td>
                                                <td><?php echo $r['received'] ?></td>
                                                <td><?php echo $r['sent'] ?></td>
                                                <td><?php echo time_elapsed_string($r['last_seen']) ?></td>
 
                                                <td class="smallText" align="right" style="white-space:nowrap;">
                                                    <a href="dock_modify.php<?php echo '?dock=' . $r['id'] ?>"><i class="fa fa-pen"></i>
                                                    </a>
                                                </td>
                                            </tr>
                                        <?php endwhile; ?>
                                    </tbody>
                                </table>
                            </div>
                        </div>
                    </div>

                    <!-- Button trigger modal -->
                    <button type="button" class="btn btn-primary" data-toggle="modal" data-target="#exampleModal">
                        Register New Boondock Echo
                    </button>


                    <!-- Modal -->
                    <div class="modal fade" id="exampleModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel" aria-hidden="true">
                        <div class="modal-dialog" role="document">
                            <div class="modal-content">
                                <div class="modal-header">
                                    <h5 class="modal-title" id="exampleModalLabel">Register New Boondock Echo</h5>
                                    <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                                        <span aria-hidden="true">&times;</span>
                                    </button>
                                </div>
                                <div class="modal-body">
                                    <div class="col-md-8">
                                        <label class="small mb-1" for="inputFrequency">MAC</label>
                                        <input class="form-control" id="inputFrequency" type="text" placeholder="Enter MAC Address" value="" />
                                    </div>
                                    <div class="col-md-8">
                                        <label class="small mb-1" for="inputFrequency">Name</label>
                                        <input class="form-control" id="inputFrequency" type="text" placeholder="Give your Boondock a friendly name" value="" />
                                    </div>
                                    <div class="col-md-6">
                                        <label class="small mb-1" for="inputFrequency">Station</label>
                                        <input class="form-control" id="inputFrequency" type="text" placeholder="e.g. Police, Fire" value="" />
                                    </div>
                                    <div class="col-md-6">
                                        <label class="small mb-1" for="inputFrequency">Frequency</label>
                                        <input class="form-control" id="inputFrequency" type="text" placeholder="Station Frequency" value="" />
                                    </div>

                                </div>



                                <div class="modal-footer">
                                    <button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>
                                    <button type="button" class="btn btn-primary">Register</button>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <!-- /.container-fluid -->



            </div>
            <!-- End of Main Content -->

            <!-- Footer -->
            <?php include '_footer.php' ?>
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
    <?php include '_logout.php' ?>
    <?php include '_bootstrap.php' ?>


</body>

</html>