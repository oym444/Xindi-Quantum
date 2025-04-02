#include "vtkconverter.h"
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <Poly_Triangulation.hxx>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkTriangle.h>
#include <TopoDS.hxx>            // 新增：包含 TopoDS 命名空间定义
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepMesh_IncrementalMesh.hxx>

vtkSmartPointer<vtkPolyData> VtkConverter::convertToVtkHighLevel(const TopoDS_Shape& shape) {
    // 使用 OpenCASCADE 的三角剖分功能
    //BRepMesh_IncrementalMesh mesh(shape, 0.1, true, 0.5, 20);
    // 更严格的剖分参数（示例）
    BRepMesh_IncrementalMesh mesh(shape, 0.01, true, 0.1); // 线性偏差 0.01，角度偏差 0.1 弧度

    // 创建 IVtkOCC_Shape 实例
    IVtkOCC_Shape::Handle occShape = new IVtkOCC_Shape(shape);
    // 创建 IVtkVTK_ShapeData 实例
    vtkSmartPointer<IVtkTools_ShapeDataSource> source = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
    source->SetShape(occShape);
    source->Update();

    return source->GetOutput();
}

vtkSmartPointer<vtkPolyData> VtkConverter::convertToVtkLowLevel(const TopoDS_Shape& shape) {
    BRepMesh_IncrementalMesh mesh(shape, 0.01, true, 0.1);
    mesh.Perform();

    vtkNew<vtkPolyData> polyData;
    vtkNew<vtkPoints> points;
    vtkNew<vtkCellArray> triangles;

    TopExp_Explorer explorer(shape, TopAbs_FACE);
    for (; explorer.More(); explorer.Next()) {
        TopoDS_Face face = TopoDS::Face(explorer.Current());
        TopLoc_Location loc;
        Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, loc);

        if (triangulation.IsNull()) {
            std::cerr << "No triangulation for face." << std::endl;
            continue;
        }

        // 获取当前顶点偏移量
        const int offset = points->GetNumberOfPoints();

        // 处理顶点
        const TColgp_Array1OfPnt& nodes = triangulation->Nodes();
        for (int i = 1; i <= nodes.Length(); ++i) {
            gp_Pnt p = nodes(i).Transformed(loc);
            points->InsertNextPoint(p.X(), p.Y(), p.Z());
        }

        // 处理三角形并调整面朝向
        const Poly_Array1OfTriangle& triArray = triangulation->Triangles();
        for (int i = 1; i <= triArray.Length(); ++i) {
            Poly_Triangle tri = triArray(i);
            int n1, n2, n3;
            tri.Get(n1, n2, n3);

            if (n1 < 1 || n1 > nodes.Length() ||
                n2 < 1 || n2 > nodes.Length() ||
                n3 < 1 || n3 > nodes.Length()) {
                std::cerr << "Invalid triangle indices." << std::endl;
                continue;
            }

            // 调整顶点顺序以处理反向面
            if (face.Orientation() == TopAbs_REVERSED) {
                std::swap(n2, n3);
            }

            vtkNew<vtkTriangle> vtkTri;
            vtkTri->GetPointIds()->SetId(0, offset + (n1 - 1));
            vtkTri->GetPointIds()->SetId(1, offset + (n2 - 1));
            vtkTri->GetPointIds()->SetId(2, offset + (n3 - 1));
            triangles->InsertNextCell(vtkTri);
        }
    }

    polyData->SetPoints(points);
    polyData->SetPolys(triangles);

    if (polyData->GetNumberOfPoints() == 0) {
        std::cerr << "Low-level conversion failed." << std::endl;
        return nullptr;
    }

    return polyData;
}
