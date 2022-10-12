<?php
include "_config.php";
include "_fx.php";
include "_auth.php";

//Get the KPI's
try {
    // execute the stored procedure
    $sql = 'CALL get_kpi( ' . $_SESSION["id"] . ' )';

    // call the stored procedure
    $q = $pdo->query($sql);
    $q->setFetchMode(PDO::FETCH_ASSOC);

    $r = $q->fetch();

    if (!isset($kpi_dock_count))
        $kpi_dock_count = $r['dock_count'];
    else
        $kpi_dock_count = 0;
    if (!isset($kpi_total_messages))
        $kpi_total_messages = $r['total_messages'];
    else
        $kpi_total_messages = 0;
    if (!isset($kpi_sent_messages))
        $kpi_sent_messages = $r['sent_messages'];
    else
        $kpi_sent_messages = 0;
    if (!isset($kpi_retransmission_rate))
        $kpi_retransmission_rate = $r['retransmission_rate'];
    else
        $kpi_retransmission_rate = 0;

    unset($sql);
    unset($q);
} catch (PDOException $e) {
    die("Error occurred:" . $e->getMessage());
}



// Get the Active messages
try {
    //  $pdo = new PDO("mysql:host=$host;dbname=$dbname", $user, $password);
    // execute the stored procedure
    $sql = 'CALL get_active_messages(' . $_SESSION["id"] . ',0,0)';

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
        <ul class="navbar-nav bg-gradient-primary sidebar sidebar-dark accordion" id="accordionSidebar">

            <?php include '_nav.php' ?>

        </ul>
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

                    <?php include '_kpi.php' ?>
                    <!-- DataTales Example -->
                    <div class="card shadow mb-4">
                        <div class="card-header py-3">
                            <!--  
                                <h6 class="m-0 font-weight-bold text-primary">Received Messages</h6>
                            -->
                            <div class="dropdown float-right">
                                <button class="btn btn-secondary dropdown-toggle" type="button" id="dropdownMenuButton" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
                                    Select Tags
                                </button>
                                <div class="dropdown-menu" aria-labelledby="dropdownMenuButton">
                                    <a class="dropdown-item" href="#">Action</a>
                                    <a class="dropdown-item" href="#">Another action</a>
                                    <a class="dropdown-item" href="#">Something else here</a>
                                </div>
                            </div>
                        </div>
                        <div class="card-body">
                            <div class="table-responsive">
                                <table class="table table-bordered" id="dataTable" width="100%" cellspacing="0">
                                    <thead>
                                        <tr>
                                            <th>Station</th>
                                            <th>Source</th>
                                            <th>Frequency</th>
                                            <th>Audio</th>
                                            <th>Received</th>
                                            <th>Size</th>
                                            <th>Status</th>
                                            <th>Action</th>
                                        </tr>
                                    </thead>

                                    <tbody>

                                        <?php while ($r = $q->fetch()) : ?>
                                            <tr>
                                                <td><?php echo $r['station'] ?></td>
                                                <td><?php echo $r['name'] ?></i></td>
                                                <td><?php echo $r['frequency'] ?></td>
                                                <td>
                                                    <audio controls style="height: 30px;">
                                                        <source src="<?php echo $r['file_name'] ?>" type="audio/wav">
                                                        Your browser does not support the audio tag.
                                                    </audio>

                                                </td>
                                                <td><?php echo time_elapsed_string($r['added']) ?></td>
                                                <td><?php echo $r['length'] ?></i></td>

                                                <td>

                                                    <a><i class="fa fa-envelope text-danger "> </i>
                                                        <a><i class="fa fa-exclamation"> </i>

                                                </td>

                                                <td>
                                                    <a href="send.php?mid=<?php echo $r['file_name'] ?>"><i class="fa fa-paper-plane"> </i>
                                                        <a><i class="fa fa-phone"> </i>
                                                        <a><i class="fa fa-ellipsis-h"> </i>
                                                </td>

                                            </tr>
                                        <?php endwhile; ?>
                                    </tbody>
                                </table>

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